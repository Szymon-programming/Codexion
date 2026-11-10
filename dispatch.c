/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 12:00:00 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/10 12:26:12 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	enqueue(t_dongle *first, t_dongle *second, t_heap_node *node)
{
	heap_push(&first->queue, node);
	if (first != second)
		heap_push(&second->queue, node);
}

void	dequeue(t_dongle *first, t_dongle *second, t_heap_node *node)
{
	heap_remove(&first->queue, node->coder_id);
	if (first != second)
		heap_remove(&second->queue, node->coder_id);
}

static void	grant(t_coder *coder, t_dongle *first, t_dongle *second)
{
	dequeue(first, second, &coder->request);
	pthread_mutex_lock(&first->dongle_mutex);
	first->is_taken = 1;
	pthread_mutex_unlock(&first->dongle_mutex);
	if (first != second)
	{
		pthread_mutex_lock(&second->dongle_mutex);
		second->is_taken = 1;
		pthread_mutex_unlock(&second->dongle_mutex);
	}
	coder->waiting = 0;
	coder->granted = 1;
	pthread_cond_broadcast(&coder->data->table_cond);
}

static void	try_grant(t_coder *coder, long now)
{
	t_dongle	*first;
	t_dongle	*second;

	select_dongle(coder, &first, &second);
	if (blocked_by_better(coder->data, coder))
		return ;
	if (dongle_usable(first, now) && dongle_usable(second, now))
		grant(coder, first, second);
}

void	dispatch(t_data *data)
{
	t_coder	*coder;
	long	now;
	int		i;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		data->coders[i].seen = 0;
		i++;
	}
	now = get_current_time_ms();
	coder = next_best(data);
	while (coder != NULL)
	{
		coder->seen = 1;
		try_grant(coder, now);
		coder = next_best(data);
	}
}
