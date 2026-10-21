#include "codexion.h"

void set_simulation_stop(t_data *data)
{
	int i;

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

int coder_compile_step(t_coder *coder)
{
	t_dongle *first;
	t_dongle *second;

	select_dongle(coder, &first, &second);
	if (!take_dongle(coder, first))
		return (0);
	if (coder->data->args.number_of_coders == 1)
	{
		precise_usleep(coder->data->args.time_to_burnout, coder->data);
		release_dongle(coder, first);
		return (0);
	}
	if (!take_dongle(coder, second))
		return (release_dongle(coder, first), (0));
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

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while (!is_simulation_stopped(coder->data))
	{
		if (!coder_compile_step(coder))
			break;
		print_log(coder, "is debugging");
		precise_usleep(coder->data->args.time_to_debug, coder->data);
		print_log(coder, "is refactoring");
		precise_usleep(coder->data->args.time_to_refactor, coder->data);
	}
	return (NULL);
}

void *monitor_routine(void *arg)
{
	t_data *data;
	int i;
	long last_compile;

	data = (t_data *)arg;
	while (!is_simulation_stopped(data))
	{
		i = 0;
		while (i < data->args.number_of_coders)
		{
			pthread_mutex_lock(&data->coders[i].state_mutex);
			last_compile = data->coders[i].last_compile_start;
			pthread_mutex_unlock(&data->coders[i].state_mutex);
			if (get_current_time_ms() - last_compile >= data->args.time_to_burnout)
			{
				set_simulation_stop(data);
				print_log(&data->coders[i], "burned out");
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int start_simulation(t_data *data)
{
	int i;

	i = 0;
	data->start_time = get_current_time_ms();
	while (i < data->args.number_of_coders)
	{
		pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]);
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
