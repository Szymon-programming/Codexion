/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_simulation_stop(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->stop_mutex);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->stop_mutex);
	i = 0;
	while (i < data->args.number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].dongle_mutex);
		pthread_cond_broadcast(&data->dongles[i].dongle_cond);
		pthread_mutex_unlock(&data->dongles[i].dongle_mutex);
		i++;
	}
}

int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_current_time_ms();
	while (i < data->args.number_of_coders)
	{
		pthread_create(&data->coders[i].thread, NULL, coder_routine,
			&data->coders[i]);
		i++;
	}
	pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
	pthread_join(data->monitor_thread, NULL);
	i = 0;
	while (i < data->args.number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	return (1);
}
