/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:53:27 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:54:03 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	handle_flag_error(const char *flag, int flag_char)
{
	if (strcmp(flag, "-") == 0) {
		printf("ft_ping: -: Name or service not known\n");
		return -1;
	}
	if (flag[0] == '-') {
		printf("ft_ping: invalid option -- '%c'\n\n", flag_char);
		print_help();
		return 1;
	}
	return 0;
}

static int	parse_flags(int ac, char **av, int *verbose, int *help)
{
	int i = 1;
	int result;

	while (i < ac)
	{
		if (strcmp(av[i], "-v") == 0)
			*verbose = 1;
		else if (strcmp(av[i], "-?") == 0) {
			*help = 1;
			print_help();
			exit(0);
		}
		else if ((result = handle_flag_error(av[i], av[i][1])) != 0)
			return result;
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
				printf("ft_ping: too many destinations\n");
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
		printf("ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	if (ac == 2 && strcmp(av[1], "-?") == 0)
	{
		print_help();
		exit(0);
	}

	result = parse_flags(ac, av, &verbose, &help);
	if (result == 1) {
		exit(0);  // Exit successfully after showing help
	}
	if (result == -1) {
		exit(2);  // Exit code 2 for invalid flags
	}

	destination = get_destination(ac, av);
	if (destination == NULL && !help)
	{
		printf("ft_ping: usage error: Destination address required\n");
		exit(2);  // Exit code 2 para falta de destino
	}

	args->flag = verbose ? "-v" : NULL;
	args->dest = destination;
	args->dest_allocated = 0;  // Flag to know if dynamically allocated
	args->verbose = verbose; // guardar estado de verbose
	
	// Soporte para IP decimal y casos especiales
	if (args->dest) {
		char ipbuf[INET_ADDRSTRLEN];
		
		// Manejar casos especiales como "192" -> "0.0.0.192"
		if (handle_special_decimal(args->dest, ipbuf, sizeof(ipbuf))) {
			args->dest = strdup(ipbuf);
			args->dest_allocated = 1;  // Mark as dynamically allocated
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
