#include "codexion.h"

static void init_values(t_arguments *arguments, char *argv[])
{
	arguments->number_of_coders = atoi(argv[1]);
	arguments->time_to_burnout = atoi(argv[2]);
	arguments->time_to_compile = atoi(argv[3]);
	arguments->time_to_debug = atoi(argv[4]);
	arguments->time_to_refactor = atoi(argv[5]);
	arguments->number_of_compiles_required = atoi(argv[6]);
	arguments->dongle_cooldown = atoi(argv[7]);
}

static void check_arguments_cont(t_arguments *arguments, char *argv[])
{
	if (arguments->number_of_compiles_required <= 0)
	{
		printf("number of required compiles can't be less or equal 0");
		exit(0);
	}
	if (arguments->dongle_cooldown <= 0)
	{
		printf("dongle cooldown (miliseconds) can't be less or equal 0");
		exit(0);
	}
	if (strcmp(argv[8], "fifo") == 0)
		arguments->scheduler = 1;
	else if (strcmp(argv[8], "edf") == 0)
		arguments->scheduler = 2;
	else
	{
		printf("value should be 'fifo' or 'edf'");
		exit(0);
	}
}

void check_arguments(t_arguments *arguments, char *argv[])
{
	init_values(arguments, argv);
	if (arguments->number_of_coders <= 0)
	{
		printf("number of coders/dongles can't be less or equal 0");
		exit(0);
	}
	if (arguments->time_to_burnout <= 0)
	{
		printf("time to burnout (miliseconds) can't be less or equal 0");
		exit(0);
	}
	if (arguments->time_to_compile <= 0)
	{
		printf("time to compile (miliseconds) can't be less or equal 0");
		exit(0);
	}
	if (arguments->time_to_refactor <= 0)
	{
		printf("time to refactor (miliseconds) can't be less or equal 0");
		exit(0);
	}
	check_arguments_cont(arguments, argv);
}
