/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:53:27 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/17 08:41:36 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static void	init_args(t_args *args)
{
	args->verbose = 0;
	args->help = 0;
	args->count = 0;
	args->dest = NULL;
	args->dest_allocated = 0;
	args->flood = 0;
	args->packet_size = 56;
	args->ttl = 64;
}

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

static int parse_count_value(const char *str)
{
	char	*endptr;
	long	count_value;

	count_value = strtol(str,  &endptr, 10);
	if (*endptr != '\0' || count_value <= 0 || count_value > INT_MAX)
		return (-1);
	return ((int)count_value);
}

static int	parse_flags(int ac, char **av, t_args *args)
{
	int i = 1;
	int result;

	while (i < ac)
	{
		if (strcmp(av[i], "-v") == 0)
			args->verbose = 1;
		else if (strcmp(av[i], "-c") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 'c'\n");
				print_help();
				return(1);
			}
			args->count = parse_count_value(av[i + 1]);
			if (args->count <= 0) {
				printf("ft_ping: invalid argument: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "-?") == 0) {
			args->help = 1;
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
		if (strcmp(av[i], "-c") == 0) {
			i += 2; //saltar "-c" y su valor
			continue;
		}
		else if (strcmp(av[i], "-v") == 0 || strcmp(av[i], "-?") == 0) {
			i++;
			continue;
		} else if (av[i][0] != '-')
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
	char	*destination = NULL;
	int		result = 0;

	// si no hay destino
	if (ac < 2) {
		printf("ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	// caso especial: solo ayuda
	if (ac == 2 && strcmp(av[1], "-?") == 0)
	{
		print_help();
		exit(0);
	}

	// inicializar args con valores por defecto
	init_args(args);
	
	// parsear flags directamente en args
	result = parse_flags(ac, av, args);
	if (result == 1)
		exit(0);  // Exit successfully after showing help
	if (result == -1)
		exit(2);  // Exit code 2 for invalid flags

	// Parsear destino
	destination = get_destination(ac, av);
	if (destination == NULL && !args->help)
	{
		printf("ft_ping: usage error: Destination address required\n");
		exit(2);  // Exit code 2 para falta de destino
	}

	// guardar destino y procesar casos especiales
	args->dest = destination;
	args->dest_allocated = 0;
	
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
