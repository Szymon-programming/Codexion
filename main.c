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
	// there is end of my journey with this task with actuall knowleadge
	// now i have to learn about parralel programming.
}