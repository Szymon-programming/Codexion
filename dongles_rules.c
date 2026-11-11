/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_rules.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 22:12:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/11/11 12:33:34 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_usable(t_dongle *dongle, long now)
{
	return (!dongle->is_taken && dongle->cooldown_end_time <= now);
}

static int	blocks(t_coder *other, t_coder *coder)
{
	return (other != coder && other->waiting && other->seen
		&& !other->left_dongle->is_taken && !other->right_dongle->is_taken);
}

int	blocked_by_better(t_data *data, t_coder *coder)
{
	int	n;
	int	left;
	int	right;

	n = data->args.number_of_coders;
	left = (coder->coder_id - 2 + n) % n;
	right = coder->coder_id % n;
	return (blocks(&data->coders[left], coder)
		|| blocks(&data->coders[right], coder));
}
