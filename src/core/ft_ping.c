/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/17 07:12:34 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help(void)
{
	const char	*help[] = {
		"Usage\n"
		"  ft_ping [options] <destination>",
		"\n",
		"Options:",
		"  <destination>\t\tDNS name or IP address",
		"  -v\t\t\tverbose output",
		"  -?\t\t\tprint help and exit",
		"  -c <count>\t\tstop after <count> replies"
		"",
		"Example:",
		"  sudo ./ft_ping google.com",
		"  sudo ./ft_ping -v google.com",
		NULL
	};
	for (int i = 0; help[i]; i++)
		puts(help[i]);
}



void	start_ping_loop(int socket_fd, struct sockaddr_in *target_addr, char *target, int mode_verbose)
{
	struct timeval	send_moment;
	t_ping_stats	stats;
	t_ping_context	ping_info;
	double			response_time;

	// Inicializar estadísticas
	setup_stats(&stats);
	
	// Configurar contexto del ping
	ping_info.socket_fd = socket_fd;
	ping_info.target_addr = target_addr;
	ping_info.packet_id = 0;
	ping_info.packet_number = 0;
	ping_info.next_number = 1;
	ping_info.stats = &stats;

	while (!g_stop)
	{
		gettimeofday(&send_moment, NULL);
		send_ping_packet(&ping_info);
		response_time = icmp_receive(ping_info.socket_fd, ping_info.packet_id, &send_moment, mode_verbose);
		handle_ping_response(&ping_info, response_time);
		if (g_stop)
			break;
		ping_info.next_number++;
		sleep(1);
	}
	print_final_stats(&stats, target);
}

static int	setup_handler(t_args *args, int ac, char **av)
{
	setup_signal_handler();
	memset(args, 0, sizeof(t_args));
	if (geteuid() != 0) {
        printf("ft_ping: This program must be run as root (RAW socket required)\n");
		cleanup_args(args);
		return 1;
	}
    parse_arguments(ac, av, args);
	return 0;
}



int	main(int ac, char **av)
{
	struct sockaddr_in	target_addr;
	t_args				args;
	int					socket_fd;

	if (setup_handler(&args, ac, av) != 0)
		return 1;

	socket_fd = create_and_validate_socket(&args);

	convert_ip_binary(socket_fd, &args, &target_addr);
	start_ping_loop(socket_fd, &target_addr, args.target, args.mode_verbose);

	close_socket(socket_fd);
	cleanup_args(&args);
	return 0;
}
