#include "codexion.h"

void shift_up(t_heap *heap, int index)
{
	int parent;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (has_higher_priority(&heap->nodes[index], &heap->nodes[parent], heap->scheduler))
			swap_nodes(&heap->nodes[index], &heap->nodes[parent]);
		else
			break;
		index = parent;
	}
}

int heap_push(t_heap *heap, t_heap_node *node)
{
	if (heap->size >= heap->capacity)
		return (0);
	heap->nodes[heap->size] = *node;
	shift_up(heap, heap->size);
	heap->size += 1;
	return (1);
}

t_heap_node *heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->nodes[0]);
}

void shift_down(t_heap *heap, int index)
{
	int left;
	int right;
	int best;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		best = index;
		if (left < heap->size && has_higher_priority(&heap->nodes[left], &heap->nodes[best], heap->scheduler))
			best = left;
		if (right < heap->size && has_higher_priority(&heap->nodes[right], &heap->nodes[best], heap->scheduler))
			best = right;
		if (best == index)
			break;
		swap_nodes(&heap->nodes[index], &heap->nodes[best]);
		index = best;
	}
}

int heap_pop(t_heap *heap)
{
	if (heap->size == 0)
		return (0);
	heap->size -= 1;
	if (heap->size > 0)
	{
		heap->nodes[0] = heap->nodes[heap->size];
		shift_down(heap, 0);
	}
	return (1);
}
