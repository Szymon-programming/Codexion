#include "codexion.h"

int main(int argc, char *argv[])
{
	t_heap heap;
	init_heap(&heap, 5, FIFO);
	t_heap_node node1 = {1, 300, 500};
	t_heap_node node2 = {2, 100, 200};
	t_heap_node node3 = {3, 800, 400};
	t_heap_node node4 = {4, 500, 200};
	heap_push(&heap, &node1);
	heap_push(&heap, &node4);
	heap_push(&heap, &node3);
	heap_push(&heap, &node2);
	while (heap.size > 0)
	{
		t_heap_node *a;
		a = heap_peek(&heap);
		printf("A id: %i, req_time: %ld, deadline: %ld\n", a->coder_id, a->request_time, a->deadline);
		heap_pop(&heap);
	}
	free_heap(&heap);
	return (0);
}