/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:53:27 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/23 11:41:53 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	setup_default_values(t_args *args)
{
	args->mode_verbose = 0;
	args->show_help = 0;
	args->packet_count = 0;
	args->target = NULL;
	args->target_was_created = 0;
	args->flood_mode = 0;
	args->packet_bytes = 56;
	args->time_to_live = 64;
	args->interval = 1.0;  // 1 segundo por defecto
	args->timeout = 3;     // 3 segundos por defecto
}

static int	handle_wrong_flag(const char *flag, int flag_char)
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

static int get_count_from_text(const char *str)
{
	char	*endptr;
	long	count_value;

	count_value = strtol(str,  &endptr, 10);
	if (*endptr != '\0' || count_value <= 0 || count_value > INT_MAX)
		return (-1);
	return ((int)count_value);
}

static int get_packet_size_from_text(const char *str)
{
	char	*endptr;
	long	size_value;

	size_value = strtol(str, &endptr, 10);
	if (*endptr != '\0' || size_value < 0 || size_value > 65507)
		return (-1);
	return ((int)size_value);
}

static int get_ttl_from_text(const char *str)
{
	char	*endptr;
	long	ttl_value;

	ttl_value = strtol(str, &endptr, 10);
	if (*endptr != '\0' || ttl_value < 1 || ttl_value > 255)
		return (-1);
	return ((int)ttl_value);
}

static double get_interval_from_text(const char *str)
{
	char	*endptr;
	double	interval_value;

	interval_value = strtod(str, &endptr);
	if (*endptr != '\0' || interval_value < 0.001)
		return (-1.0);
	return (interval_value);
}

static int get_timeout_from_text(const char *str)
{
	char	*endptr;
	long	timeout_value;

	timeout_value = strtol(str, &endptr, 10);
	if (*endptr != '\0' || timeout_value < 1 || timeout_value > 3600)
		return (-1);
	return ((int)timeout_value);
}

static int	check_all_flags(int ac, char **av, t_args *args)
{
	int i = 1;
	int result;

	while (i < ac)
	{
		if (strcmp(av[i], "-v") == 0)
			args->mode_verbose = 1;
		else if (strcmp(av[i], "-f") == 0)
			args->flood_mode = 1;
		else if (strcmp(av[i], "-c") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 'c'\n");
				print_help();
				return(1);
			}
			args->packet_count = get_count_from_text(av[i + 1]);
			if (args->packet_count <= 0) {
				printf("ft_ping: invalid argument: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "-s") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 's'\n");
				print_help();
				return(1);
			}
			args->packet_bytes = get_packet_size_from_text(av[i + 1]);
			if (args->packet_bytes < 0) {
				printf("ft_ping: invalid packet size: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "--ttl") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 'ttl'\n");
				print_help();
				return(1);
			}
			args->time_to_live = get_ttl_from_text(av[i + 1]);
			if (args->time_to_live < 0) {
				printf("ft_ping: invalid TTL: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "-i") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 'i'\n");
				print_help();
				return(1);
			}
			args->interval = get_interval_from_text(av[i + 1]);
			if (args->interval < 0) {
				printf("ft_ping: invalid interval: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "-W") == 0) {
			if (i + 1 >= ac) {
				printf("ft_ping: option requires an argument -- 'W'\n");
				print_help();
				return(1);
			}
			args->timeout = get_timeout_from_text(av[i + 1]);
			if (args->timeout < 0) {
				printf("ft_ping: invalid timeout: '%s'\n", av[i + 1]);
				return 1;
			}
			i++;
		}
		else if (strcmp(av[i], "-?") == 0) {
			args->show_help = 1;
			print_help();
			exit(0);
		}
		else if ((result = handle_wrong_flag(av[i], av[i][1])) != 0)
			return result;
		i++;
	}
	return 0;
}

static char	*get_target_from_args(int ac, char **av)
{
	int i;
	char *target;

	target = NULL;
	i = 1;
	while (i < ac)
	{
		if (strcmp(av[i], "-c") == 0 || strcmp(av[i], "-s") == 0 || 
			strcmp(av[i], "--ttl") == 0 || strcmp(av[i], "-i") == 0 || 
			strcmp(av[i], "-W") == 0) {
			i += 2; //saltar flag y su valor
			continue;
		}
		else if (strcmp(av[i], "-v") == 0 || strcmp(av[i], "-?") == 0 || 
				 strcmp(av[i], "-f") == 0) {
			i++;
			continue;
		} else if (av[i][0] != '-')
		{
			if (target != NULL)
			{
				printf("ft_ping: too many destinations\n");
				exit(2);  // Exit code 2 para demasiados argumentos
			}
			target = av[i];
		}
		i++;
	}
	return target;
}

static void	process_all_arguments(int ac, char **av, t_args *args)
{
	char	*target = NULL;
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
	setup_default_values(args);
	
	// parsear flags directamente en args
	result = check_all_flags(ac, av, args);
	if (result == 1)
		exit(0);  // Exit successfully after showing help
	if (result == -1)
		exit(2);  // Exit code 2 for invalid flags

	// Parsear destino
	target = get_target_from_args(ac, av);
	if (target == NULL && !args->show_help)
	{
		printf("ft_ping: usage error: Destination address required\n");
		exit(2);  // Exit code 2 para falta de destino
	}

	// guardar destino y procesar casos especiales
	args->target = target;
	args->target_was_created = 0;
	
	// Soporte para IP decimal y casos especiales
	if (args->target) {
		char ipbuf[INET_ADDRSTRLEN];
		
		// Manejar casos especiales como "192" -> "0.0.0.192"
		if (handle_special_decimal(args->target, ipbuf, sizeof(ipbuf))) {
			args->target = strdup(ipbuf);
			args->target_was_created = 1;  // Mark as dynamically allocated
		}
	}
}

void	parse_arguments(int ac, char **av, t_args *args)
{
	process_all_arguments(ac, av, args);
	if (args == NULL)
		return ;
	validate_destination(args);
}
