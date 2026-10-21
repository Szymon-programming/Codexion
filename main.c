#include "codexion.h"

int main(int argc, char *argv[])
{
	t_data data;

	memset(&data.args, 0, sizeof(t_arguments));
	if (argc != 9)
	{
		printf("not enought/too much arguments");
		return (1);
	}
	if (!check_arguments(&data.args, argv))
		return (1);
	if (!init_data(&data))
		return (1);
	if (!init_dongles(&data))
		return (1);
	data.start_time = get_current_time_ms();
	init_coders(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
