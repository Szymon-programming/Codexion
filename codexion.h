#ifndef CODEXION_H
#define CODEXION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

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

typedef struct s_heap_node
{
	int coder_id;
	long request_time;
	long deadline;
} t_heap_node;

typedef struct s_heap
{
	t_heap_node *nodes;
	int size;
	int capacity;
	t_scheduler scheduler;
} t_heap;

typedef struct s_dongle
{
	int dongle_id;
	int is_taken;
	long cooldown_end_time;
	pthread_mutex_t dongle_mutex;
	pthread_cond_t dongle_cond;
	t_heap queue;
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

// validator
void check_arguments(t_arguments *arguments, char *argv[]);

// heap functions for EDF and FIFO:
int init_heap(t_heap *heap, int capacity, t_scheduler scheduler);
void free_heap(t_heap *heap);
void swap_nodes(t_heap_node *a, t_heap_node *b);
int has_higher_priority(t_heap_node *a, t_heap_node *b, t_scheduler scheduler);
void shift_up(t_heap *heap, int index);
int heap_push(t_heap *heap, t_heap_node *node);
t_heap_node *heap_peek(t_heap *heap);
void shift_down(t_heap *heap, int index);
int heap_pop(t_heap *heap);

// time:
long get_current_time_ms(void);
long get_elapsed_time_ms(long start_time);
void precise_usleep(long sleep_time_ms, t_data *data);

int is_simulation_stopped(t_data *data);
void print_log(t_coder *coder, char *status);

#endif