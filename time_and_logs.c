/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_logs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/07 00:08:39 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_ms	get_elapsed_time_ms(t_ms start_time)
{
	t_ms	current;

	current = get_current_time_ms();
	return (current - start_time);
}

t_ms	get_current_time_ms(void)
{
	struct timeval	tv;
	t_ms			sum;

	gettimeofday(&tv, NULL);
	sum = ((t_ms)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (sum);
}

int	is_simulation_stopped(t_data *data)
{
	int	is_stopped;

	pthread_mutex_lock(&data->stop_mutex);
	is_stopped = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_mutex);
	return (is_stopped);
}

void	precise_usleep(t_ms sleep_time_ms, t_data *data)
{
	t_ms	sleep_start;

	sleep_start = get_current_time_ms();
	while (!is_simulation_stopped(data))
	{
		if (get_current_time_ms() - sleep_start >= sleep_time_ms)
			break ;
		usleep(500);
	}
}

void	print_log(t_coder *coder, char *status)
{
	t_ms	timestamp;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (!is_simulation_stopped(coder->data))
	{
		timestamp = get_elapsed_time_ms(coder->data->start_time);
		printf("%lld %d %s\n", timestamp, coder->coder_id, status);
	}
	pthread_mutex_unlock(&coder->data->log_mutex);
}
