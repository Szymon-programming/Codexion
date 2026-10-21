#include "codexion.h"

static long get_elapsed_time_ms(long start_time)
{
	long current;
	current = get_current_time_ms();
	return (current - start_time);
}

long get_current_time_ms(void)
{
	struct timeval tv;
	long sum;
	gettimeofday(&tv, NULL);
	sum = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (sum);
}

int is_simulation_stopped(t_data *data)
{
	int is_stopped;

	pthread_mutex_lock(&data->stop_mutex);
	is_stopped = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_mutex);
	return (is_stopped);
}

void precise_usleep(long sleep_time_ms, t_data *data)
{
	long sleep_start;

	sleep_start = get_current_time_ms();
	while (!is_simulation_stopped(data))
	{
		if (get_current_time_ms() - sleep_start >= sleep_time_ms)
			break;
		usleep(500);
	}
}

void print_log(t_coder *coder, char *status)
{
	long timestamp;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (!is_simulation_stopped(coder->data))
	{
		timestamp = get_elapsed_time_ms(coder->data->start_time);
		printf("%ld %d %s\n", timestamp, coder->coder_id, status);
	}
	pthread_mutex_unlock(&coder->data->log_mutex);
}
