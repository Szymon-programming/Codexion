/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schoinsk <schoinsk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:48:08 by schoinsk          #+#    #+#             */
/*   Updated: 2026/10/21 21:48:08 by schoinsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_data	data;

	memset(&data.args, 0, sizeof(t_arguments));
	if (argc != 9)
	{
		printf("not enough/too much arguments\n");
		return (1);
	}
	if (check_arguments(&data.args, argv))
		return (1);
	if (!init_data(&data))
		return (1);
	if (!init_dongles(&data))
		return (1);
	data.start_time = get_current_time_ms();
	init_coders(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
