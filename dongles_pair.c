/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_pair.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/11 12:33:34 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	t_data	*data;

	data = coder->data;
	init_node_request(coder, &coder->request);
	pthread_mutex_lock(&data->table_mutex);
	coder->waiting = 1;
	coder->granted = 0;
	heap_push(&data->waiting_queue, &coder->request);
	dispatch(data);
	while (!coder->granted && !is_simulation_stopped(data))
		pthread_cond_wait(&data->table_cond, &data->table_mutex);
	if (!coder->granted)
	{
		heap_remove(&data->waiting_queue, coder->coder_id);
		coder->waiting = 0;
		pthread_mutex_unlock(&data->table_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->table_mutex);
	print_log(coder, "has taken a dongle");
	if (first != second)
		print_log(coder, "has taken a dongle");
	return (1);
}
