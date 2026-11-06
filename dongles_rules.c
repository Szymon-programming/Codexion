/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_rules.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_usable(t_dongle *dongle, long now)
{
	return (!dongle->is_taken && dongle->cooldown_end_time <= now);
}

static int	coder_eligible(t_data *data, int coder_id, long now)
{
	t_coder	*coder;

	coder = &data->coders[coder_id - 1];
	return (dongle_usable(coder->left_dongle, now)
		&& dongle_usable(coder->right_dongle, now));
}

static int	blocked_by_better(t_data *data, t_heap_node *me, t_dongle *dongle,
		long now)
{
	int	i;

	i = 0;
	while (i < dongle->queue.size)
	{
		if (dongle->queue.nodes[i].coder_id != me->coder_id
			&& has_higher_priority(&dongle->queue.nodes[i], me,
				dongle->queue.scheduler)
			&& coder_eligible(data, dongle->queue.nodes[i].coder_id, now))
			return (1);
		i++;
	}
	return (0);
}

int	can_take(t_data *data, t_heap_node *me, t_dongle *first, t_dongle *second)
{
	long	now;

	now = get_current_time_ms();
	if (!dongle_usable(first, now) || !dongle_usable(second, now))
		return (0);
	if (blocked_by_better(data, me, first, now))
		return (0);
	return (!blocked_by_better(data, me, second, now));
}
