#ifndef CODEXION_H
#define CODEXION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
} t_scheduler;

typedef struct s_arguments
{
	int number_of_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	t_scheduler scheduler;
} t_arguments;

typedef struct s_dongle
{
	int dongle_id;
	int is_taken;
	long cooldown_end_time;
	pthread_mutex_t dongle_mutex;
	pthread_cond_t dongle_cond;
	// heap of waiting tasks, but i have to first think about heap structure
} t_dongle;

typedef struct s_coder
{
	int coder_id;
	pthread_t thread;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	long last_compile_start;
	int compiles_count;
	pthread_mutex_t state_mutex;
	struct s_data *data;
} t_coder;

typedef struct s_data
{
	t_arguments args;			// input data after parsing
	long start_time;			// program start time
	t_coder *coders;			// dynamically allocated coders list
	t_dongle *dongles;			// dynamically allocated dongles list
	pthread_t monitor_thread;	// thread carrier monitoring burnout
	int stop_simulation;		// flag informing programm, that simulation has ended
	pthread_mutex_t stop_mutex; // mutex defendining flag stop_simulation
	pthread_mutex_t log_mutex;	// dedicated mutex for defending of wrtining on screen with printf so that two messages never interleave on a single line
} t_data;

void check_arguments(t_arguments *arguments, char *argv[]);

#endif