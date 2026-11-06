/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_pair.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>

static void	wait_table(t_data *data)
{
	struct timeval	now;
	struct timespec	until;

	gettimeofday(&now, NULL);
	until.tv_sec = now.tv_sec;
	until.tv_nsec = (now.tv_usec + 1000) * 1000L;
	if (until.tv_nsec >= 1000000000L)
	{
		until.tv_sec += 1;
		until.tv_nsec -= 1000000000L;
	}
	pthread_cond_timedwait(&data->table_cond, &data->table_mutex, &until);
}

static void	enqueue(t_dongle *first, t_dongle *second, t_heap_node *node)
{
	heap_push(&first->queue, node);
	if (first != second)
		heap_push(&second->queue, node);
}

static void	dequeue(t_dongle *first, t_dongle *second, t_heap_node *node)
{
	heap_remove(&first->queue, node->coder_id);
	if (first != second)
		heap_remove(&second->queue, node->coder_id);
}

static void	mark_taken(t_dongle *first, t_dongle *second, int value)
{
	pthread_mutex_lock(&first->dongle_mutex);
	first->is_taken = value;
	pthread_mutex_unlock(&first->dongle_mutex);
	if (first != second)
	{
		pthread_mutex_lock(&second->dongle_mutex);
		second->is_taken = value;
		pthread_mutex_unlock(&second->dongle_mutex);
	}
}

int	take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	t_heap_node	node;
	t_data		*data;

	data = coder->data;
	init_node_request(coder, &node);
	pthread_mutex_lock(&data->table_mutex);
	enqueue(first, second, &node);
	while (!is_simulation_stopped(data)
		&& !can_take(data, &node, first, second))
		wait_table(data);
	dequeue(first, second, &node);
	if (is_simulation_stopped(data))
	{
		pthread_mutex_unlock(&data->table_mutex);
		return (0);
	}
	mark_taken(first, second, 1);
	pthread_mutex_unlock(&data->table_mutex);
	print_log(coder, "has taken a dongle");
	if (first != second)
		print_log(coder, "has taken a dongle");
	return (1);
}
