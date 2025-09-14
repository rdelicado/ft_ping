/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/14 10:17:40 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help(void)
{
	const char	*help[] = {
		"Usage\n"
		"  ft_ping [options] <destination>",
		"\n"
		"Options:",
		"  <destination>		DNS name or IP address",
		"  -v			verbose output",
		"  -?			print help and exit",
		"",
		"Example:",
		"  sudo ./ft_ping google.com",
		"  sudo ./ft_ping -v google.com",
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
	
	cleanup_args(&args);
	
	return 0;
}
