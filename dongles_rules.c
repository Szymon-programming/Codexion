/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_rules.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 22:12:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/10 12:00:00 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_usable(t_dongle *dongle, long now)
{
	return (!dongle->is_taken && dongle->cooldown_end_time <= now);
}

static int	shares_dongle(t_coder *a, t_coder *b)
{
	return (a->left_dongle == b->left_dongle
		|| a->left_dongle == b->right_dongle
		|| a->right_dongle == b->left_dongle
		|| a->right_dongle == b->right_dongle);
}

/* a waiting neighbour with higher priority whose dongles are both still
** free (cooldown may be running) keeps this coder off the shared dongle */
int	blocked_by_better(t_data *data, t_coder *coder)
{
	int		i;
	t_coder	*other;

	i = 0;
	while (i < data->args.number_of_coders)
	{
		other = &data->coders[i];
		if (other != coder && other->waiting && shares_dongle(other, coder)
			&& has_higher_priority(&other->request, &coder->request,
				data->args.scheduler)
			&& !other->left_dongle->is_taken && !other->right_dongle->is_taken)
			return (1);
		i++;
	}
	return (0);
}

t_coder	*next_best(t_data *data)
{
	int		i;
	t_coder	*best;
	t_coder	*c;

	i = 0;
	best = NULL;
	while (i < data->args.number_of_coders)
	{
		c = &data->coders[i];
		if (c->waiting && !c->seen && (best == NULL
				|| has_higher_priority(&c->request, &best->request,
					data->args.scheduler)))
			best = c;
		i++;
	}
	return (best);
}
