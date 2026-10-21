#include "codexion.h"

void *monitor_routine(void *arg)
{
	t_data *data;
	int i;

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
