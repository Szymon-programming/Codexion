/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 11:37:57 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/10 12:03:20 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_data(t_data *data)
{
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->table_mutex);
	pthread_cond_destroy(&data->table_cond);
	free_heap(&data->waiting_queue);
	free(data->coders);
	free(data->dongles);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].dongle_mutex);
		pthread_mutex_destroy(&data->coders[i].state_mutex);
		i++;
	}
	free_data(data);
}
