#ifndef CODEXION_H
#define CODEXION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct s_arguments
{
	int number_of_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	int scheduler;
} t_arguments;

void check_arguments(t_arguments *arguments, char *argv[]);

#endif