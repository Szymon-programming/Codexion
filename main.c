#include "codexion.h"

int main(int argc, char *argv[])
{
	t_arguments arguments;

	memset(&arguments, 0, sizeof(t_arguments));
	if (argc != 9)
	{
		printf("not enought/too much arguments");
		exit(0);
	}
	// sprawdzam argumenty:
	check_arguments(&arguments, argv);
	printf("%d\n", arguments.number_of_coders);
	printf("%d\n", arguments.time_to_burnout);
	printf("%d\n", arguments.scheduler);
}