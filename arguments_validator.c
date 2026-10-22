/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_validator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>

static int	parse_int(char *str, int *value)
{
	int	result;
	int	digit;
	int	i;

	if (str[0] == '\0')
		return (0);
	result = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (result > (INT_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*value = result;
	return (1);
}

static int	parse_values(t_arguments *arguments, char *argv[])
{
	if (!parse_int(argv[1], &arguments->number_of_coders)
		|| !parse_int(argv[2], &arguments->time_to_burnout)
		|| !parse_int(argv[3], &arguments->time_to_compile)
		|| !parse_int(argv[4], &arguments->time_to_debug)
		|| !parse_int(argv[5], &arguments->time_to_refactor)
		|| !parse_int(argv[6], &arguments->number_of_compiles_required)
		|| !parse_int(argv[7], &arguments->dongle_cooldown))
	{
		printf("arguments must be integers from 0 to %d\n", INT_MAX);
		return (0);
	}
	return (1);
}

static int	check_scheduler(t_arguments *arguments, char *argv[])
{
	if (strcmp(argv[8], "fifo") == 0)
		arguments->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		arguments->scheduler = EDF;
	else
	{
		printf("value should be 'fifo' or 'edf'\n");
		return (1);
	}
	return (0);
}

int	check_arguments(t_arguments *arguments, char *argv[])
{
	if (!parse_values(arguments, argv))
		return (1);
	if (arguments->number_of_coders <= 0)
	{
		printf("number of coders/dongles can't be less or equal 0\n");
		return (1);
	}
	if (arguments->time_to_burnout <= 0)
	{
		printf("time to burnout (miliseconds) can't be less or equal 0\n");
		return (1);
	}
	if (arguments->number_of_compiles_required <= 0)
	{
		printf("number of required compiles can't be less or equal 0\n");
		return (1);
	}
	return (check_scheduler(arguments, argv));
}
