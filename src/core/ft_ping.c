/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/13 15:53:46 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help(void)
{
	const char	*help[] = {
		"Usage: ft_ping [options] <destination>",
		"Options:",
		" -v		: verbose output",
		" -?		: show this help message",
		"",
		"Example:",
		" sudo ./ft_ping google.com",
		NULL
	};

	for (int i = 0; help[i]; i++)
		puts(help[i]);
}

void	cleanup_args(t_args *args)
{
	if (args && args->dest && args->dest_allocated) {
		free(args->dest);
		args->dest = NULL;
		args->dest_allocated = 0;
	}
}

int main(int ac, char **av)
{
	t_args	args;

	memset(&args, 0, sizeof(t_args));
	parse_arguments(ac, av, &args);
	
	// Limpiar memoria asignada dinámicamente
	cleanup_args(&args);
	
	return 0;
}
