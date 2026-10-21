/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile_with_dongles(t_coder *coder, t_dongle *first,
		t_dongle *second)
{
	if (!take_dongle(coder, second))
	{
		release_dongle(coder, first);
		return (0);
	}
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_current_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	print_log(coder, "is compiling");
	precise_usleep(coder->data->args.time_to_compile, coder->data);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongle(coder, second);
	release_dongle(coder, first);
	return (1);
}

static int	coder_compile_step(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	select_dongle(coder, &first, &second);
	if (!take_dongle(coder, first))
		return (0);
	if (coder->data->args.number_of_coders == 1)
	{
		precise_usleep(coder->data->args.time_to_burnout, coder->data);
		release_dongle(coder, first);
		return (0);
	}
	return (compile_with_dongles(coder, first, second));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_simulation_stopped(coder->data))
	{
		if (!coder_compile_step(coder))
			break ;
		print_log(coder, "is debugging");
		precise_usleep(coder->data->args.time_to_debug, coder->data);
		print_log(coder, "is refactoring");
		precise_usleep(coder->data->args.time_to_refactor, coder->data);
	}
	return (NULL);
}

int	all_coders_compiled(t_data *data)
{
	int	i;

	i = 0;
	if (data->args.number_of_compiles_required <= 0)
		return (0);
	while (i < data->args.number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].state_mutex);
		if (data->coders[i].compiles_count
			< data->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->coders[i].state_mutex);
			return (0);
		}
		pthread_mutex_unlock(&data->coders[i].state_mutex);
		i++;
	}
	return (1);
}
