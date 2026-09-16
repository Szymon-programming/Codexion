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

typedef struct s_coder
{
	int coder_id;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	long last_compile_start;
	int compiles_count;
	// needs actualisation after i will get to know more about
	// parralel programming
} t_coder;

typedef struct s_dongle
{
	int dongle_id;
	// state mutex
	int is_taken;
	int cooldown_end_time;
	// t_heap priority queue
	// i have to rethink what to add more here
} t_dongle;

void check_arguments(t_arguments *arguments, char *argv[]);

#endif