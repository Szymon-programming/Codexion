/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_heap *heap, int capacity, t_scheduler scheduler)
{
	heap->size = 0;
	heap->capacity = capacity;
	heap->scheduler = scheduler;
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (heap->nodes == NULL)
		return (0);
	return (1);
}

void	free_heap(t_heap *heap)
{
	if (heap->nodes != NULL)
	{
		free(heap->nodes);
		heap->nodes = NULL;
	}
}

void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	temp;

	temp = *b;
	*b = *a;
	*a = temp;
}

int	has_higher_priority(t_heap_node *a, t_heap_node *b, t_scheduler scheduler)
{
	if (scheduler == FIFO)
	{
		if (a->request_time < b->request_time)
			return (1);
		else if (a->request_time == b->request_time
			&& a->coder_id < b->coder_id)
			return (1);
	}
	else if (scheduler == EDF)
	{
		if (a->deadline < b->deadline)
			return (1);
		else if (a->deadline == b->deadline
			&& a->coder_id < b->coder_id)
			return (1);
	}
	return (0);
}
