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
}

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while (!is_simulation_stopped(coder->data))
	{
		// dongles and compile logic
		precise_usleep(1, coder->data);
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
