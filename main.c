#include "codexion.h"

int main(int argc, char *argv[])
{
	t_arguments arguments;

	memset(&arguments, 0, sizeof(t_arguments));
	if (argc == 1)
		exit(0);
	// najpierw muszę te argumenty przekazać i zmienić stringi na inty
	// sprawdzam argumenty:
	check_arguments(&arguments, argv);
}