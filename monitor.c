/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_coder_burnout(t_data *data, int i)
{
	long	last_compile;

	pthread_mutex_lock(&data->coders[i].state_mutex);
	last_compile = data->coders[i].last_compile_start;
	pthread_mutex_unlock(&data->coders[i].state_mutex);
	if (get_current_time_ms() - last_compile >= data->args.time_to_burnout)
	{
		print_log(&data->coders[i], "burned out");
		set_simulation_stop(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!is_simulation_stopped(data))
	{
		i = 0;
		while (i < data->args.number_of_coders)
		{
			if (check_coder_burnout(data, i))
				return (NULL);
			i++;
		}
		if (all_coders_compiled(data))
		{
			set_simulation_stop(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
