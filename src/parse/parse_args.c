/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:53:27 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/13 15:36:50 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	print_error(const char *msg)
{
	printf("Error: %s\n", msg);
}

static int	parse_flags(int ac, char **av, int *verbose, int *help)
{
	int i;

	i = 1;
	while (i < ac)
	{
		if (strcmp(av[i], "-v") == 0)
			*verbose = 1;
		else if (strcmp(av[i], "-?") == 0)
		{
			*help = 1;
			print_help();
			return 1;  // Indica que se mostró ayuda
		}
		else if (av[i][0] == '-')
		{
			print_error("flag inválido");
			return -1;  // Error
		}
		i++;
	}
	return 0;
}

static char	*get_destination(int ac, char **av)
{
	int i;
	char *destination;

	destination = NULL;
	i = 1;
	while (i < ac)
	{
		if (av[i][0] != '-' && strcmp(av[i], "-?") != 0)
		{
			if (destination != NULL)
			{
				print_error("demasiados destinos");
				exit(2);  // Exit code 2 para demasiados argumentos
			}
			destination = av[i];
		}
		i++;
	}
	return destination;
}

static void	print_arguments(int ac, char **av, t_args *args)
{
	int		verbose = 0;
	int		help = 0;
	char	*destination = NULL;
	int		result = 0;

	if (ac < 2) {
		print_error("falta destino o opción");
		exit(2);
	}

	if (ac == 2 && strcmp(av[1], "-?") == 0)
	{
		print_help();
		args->flag = av[1];
		args->dest = NULL;
		return;
	}

	result = parse_flags(ac, av, &verbose, &help);
	if (result == 1) {
		args->flag = NULL;
		args->dest = NULL;
		return;
	}
	if (result == -1) {
		exit(2);  // Exit code 2 para flags inválidos
	}

	destination = get_destination(ac, av);
	if (destination == NULL && !help)
	{
		print_error("falta destino");
		exit(2);  // Exit code 2 para falta de destino
	}

	if (verbose)
		printf("Modo verbose activado\n");

	args->flag = verbose ? "-v" : NULL;
	args->dest = destination;

	// Soporte para IP decimal
	if (args->dest && is_decimal_format(args->dest)) {
		char ipbuf[INET_ADDRSTRLEN];
		if (decimal_to_ip(args->dest, ipbuf, sizeof(ipbuf))) {
			args->dest = strdup(ipbuf);
		}
	}
}

void	parse_arguments(int ac, char **av, t_args *args)
{
	print_arguments(ac, av, args);
	if (args == NULL)
		return ;
	validate_destination(args);
}
