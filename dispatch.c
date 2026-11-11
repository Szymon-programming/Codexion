/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 12:00:00 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/11 12:33:34 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	grant(t_coder *coder, t_dongle *first, t_dongle *second)
{
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

static void	restore_waiting(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		coder = &data->coders[i];
		if (coder->seen && coder->waiting)
			heap_push(&data->waiting_queue, &coder->request);
		coder->seen = 0;
		i++;
	}
}

void	dispatch(t_data *data)
{
	t_heap_node	*top;
	t_coder		*coder;
	long		now;

	now = get_current_time_ms();
	top = heap_peek(&data->waiting_queue);
	while (top != NULL)
	{
		coder = &data->coders[top->coder_id - 1];
		heap_pop(&data->waiting_queue);
		coder->seen = 1;
		try_grant(coder, now);
		top = heap_peek(&data->waiting_queue);
	}
	restore_waiting(data);
}
