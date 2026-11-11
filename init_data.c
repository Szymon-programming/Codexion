/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/10 11:45:46 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_data(t_data *data)
{
	data->stop_simulation = 0;
	data->coders = malloc(sizeof(t_coder) * data->args.number_of_coders);
	if (data->coders == NULL)
		return (0);
	data->dongles = malloc(sizeof(t_dongle) * data->args.number_of_coders);
	if (data->dongles == NULL)
	{
		free(data->coders);
		return (0);
	}
	if (!init_heap(&data->waiting_queue, data->args.number_of_coders,
			data->args.scheduler))
	{
		free(data->coders);
		free(data->dongles);
		return (0);
	}
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->table_mutex, NULL);
	pthread_cond_init(&data->table_cond, NULL);
	return (1);
}

int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		data->dongles[i].dongle_id = i + 1;
		data->dongles[i].is_taken = 0;
		data->dongles[i].cooldown_end_time = 0;
		pthread_mutex_init(&data->dongles[i].dongle_mutex, NULL);
		i++;
	}
	return (1);
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		data->coders[i].coder_id = i + 1;
		data->coders[i].compiles_count = 0;
		data->coders[i].last_compile_start = data->start_time;
		data->coders[i].data = data;
		data->coders[i].waiting = 0;
		data->coders[i].granted = 0;
		data->coders[i].seen = 0;
		pthread_mutex_init(&data->coders[i].state_mutex, NULL);
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle
			= &data->dongles[(i + 1) % data->args.number_of_coders];
		i++;
	}
	return (1);
}
