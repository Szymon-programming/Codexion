#include "codexion.h"

void free_heap(t_heap *heap)
{
	if (heap->nodes != NULL)
	{
		free(heap->nodes);
		heap->nodes = NULL;
	}
}

void swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node temp;
	temp = *b;

	*b = *a;
	*a = temp;
}

int init_heap(t_heap *heap, int capacity, t_scheduler scheduler)
{
	heap->size = 0;
	heap->capacity = capacity;
	heap->scheduler = scheduler;
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (heap->nodes == NULL)
		return (0);
	return 1;
}

int main(int argc, char *argv[])
{
	t_heap_node *a;
	a = malloc(sizeof(t_heap_node));
	t_heap_node *b;
	b = malloc(sizeof(t_heap_node));
	a->coder_id = 1;
	a->deadline = 400;
	a->request_time = 200;
	b->coder_id = 2;
	b->deadline = 300;
	b->request_time = 500;
	swap_nodes(a, b);
	printf("A id: %i, req_time: %ld, deadline: %ld\n", a->coder_id, a->request_time, a->deadline);
	printf("B id: %i, req_time: %ld, deadline: %ld", b->coder_id, b->request_time, b->deadline);
	return 0;
}
