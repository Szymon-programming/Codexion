#include "codexion.h"

void select_dongle(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

static void init_node_request(t_coder *coder, t_heap_node *node)
{
	node->coder_id = coder->coder_id;
	node->request_time = get_current_time_ms();
	pthread_mutex_lock(&coder->state_mutex);
	node->deadline = coder->last_compile_start + coder->data->args.time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
}

int take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_heap_node node;
	long wait_cooldown;

	init_node_request(coder, &node);
	pthread_mutex_lock(&dongle->dongle_mutex);
	heap_push(&dongle->queue, &node);
	while (!is_simulation_stopped(coder->data) && (dongle->is_taken || heap_peek(&dongle->queue)->coder_id != coder->coder_id))
		pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	if (is_simulation_stopped(coder->data))
	{
		pthread_mutex_unlock(&dongle->dongle_mutex);
		return (0);
	}
	dongle->is_taken = 1;
	heap_pop(&dongle->queue);
	wait_cooldown = dongle->cooldown_end_time - get_current_time_ms();
	pthread_mutex_unlock(&dongle->dongle_mutex);
	if (wait_cooldown > 0)
		precise_usleep(wait_cooldown, coder->data);
	print_log(coder, "has taken a dongle");
	return (1);
}

void release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_taken = 0;
	dongle->cooldown_end_time = get_current_time_ms() + coder->data->args.dongle_cooldown;
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
}
int all_coders_compiled(t_data *data)
{
	int i;

	i = 0;
	if (data->args.number_of_compiles_required <= 0)
		return (0);
	while (i < data->args.number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].state_mutex);
		if (data->coders[i].compiles_count < data->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->coders[i].state_mutex);
			return (0);
		}
		pthread_mutex_unlock(data->coders[i].state_mutex);
		i++;
	}
	return (1);
}
