/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_logic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/07 00:08:39 by schoinsk         ###   ########.fr       */
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

void	init_node_request(t_coder *coder, t_heap_node *node)
{
	node->coder_id = coder->coder_id;
	node->request_time = get_current_time_ms();
	pthread_mutex_lock(&coder->state_mutex);
	node->deadline = coder->last_compile_start
		+ coder->data->args.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	t_data	*data;

	data = coder->data;
	pthread_mutex_lock(&data->table_mutex);
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_taken = 0;
	dongle->cooldown_end_time = get_current_time_ms()
		+ data->args.dongle_cooldown;
	pthread_mutex_unlock(&dongle->dongle_mutex);
	dispatch(data);
	pthread_mutex_unlock(&data->table_mutex);
}
