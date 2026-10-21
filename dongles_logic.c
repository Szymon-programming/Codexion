/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_logic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	select_dongle(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

static void	init_node_request(t_coder *coder, t_heap_node *node)
{
	node->coder_id = coder->coder_id;
	node->request_time = get_current_time_ms();
	pthread_mutex_lock(&coder->state_mutex);
	node->deadline = coder->last_compile_start
		+ coder->data->args.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
}

static int	wait_for_turn(t_coder *coder, t_dongle *dongle)
{
	while (!is_simulation_stopped(coder->data)
		&& (dongle->is_taken
			|| heap_peek(&dongle->queue)->coder_id != coder->coder_id))
		pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	return (!is_simulation_stopped(coder->data));
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_heap_node	node;
	long		wait_cooldown;

	init_node_request(coder, &node);
	pthread_mutex_lock(&dongle->dongle_mutex);
	heap_push(&dongle->queue, &node);
	if (!wait_for_turn(coder, dongle))
	{
		pthread_mutex_unlock(&dongle->dongle_mutex);
		return (0);
	}
	dongle->is_taken = 1;
	heap_pop(&dongle->queue);
	wait_cooldown = dongle->cooldown_end_time - get_current_time_ms();
	pthread_mutex_unlock(&dongle->dongle_mutex);
	if (wait_cooldown > 0)
		precise_usleep(wait_cooldown, coder->data);
	print_log(coder, "has taken a dongle");
	return (1);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_taken = 0;
	dongle->cooldown_end_time = get_current_time_ms()
		+ coder->data->args.dongle_cooldown;
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
}
