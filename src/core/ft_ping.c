/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/10/24 17:46:11 by rdelicad         ###   ########.fr       */
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
		"  -c <count>\t\tstop after <count> replies",
		"  -f\t\t\tflood ping (send packets as fast as possible)",
		"  -s <size>\t\tspecify packet size in bytes (default 56)",
		"  --ttl <ttl>\t\tset Time To Live (1-255, default 64)",
		"  -i <interval>\t\tinterval between packets in seconds (default 1.0)",
		"  -W <timeout>\t\ttimeout for replies in seconds (default 3)",
		"",
		"Examples:",
		"  sudo ./ft_ping google.com",
		"  sudo ./ft_ping -v -c 3 google.com",
		"  sudo ./ft_ping -s 1000 -i 0.5 google.com",
		"  sudo ./ft_ping -f --ttl 128 google.com",
		NULL
	};
	for (int i = 0; help[i]; i++)
		puts(help[i]);
}



void	start_ping_loop(int socket_fd, struct sockaddr_in *target_addr, t_args *args)
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
	ping_info.args = args;

	// Obtener el número de paquetes a enviar
	int max_packets = 0;
	if (args->packet_count > 0)
		max_packets = args->packet_count;

	while (!g_stop && (max_packets == 0 || ping_info.next_number <= max_packets))
	{
		gettimeofday(&send_moment, NULL);
		send_ping_packet(&ping_info);
		response_time = icmp_receive(ping_info.socket_fd, ping_info.packet_id, &send_moment, args->mode_verbose);
		
		// Handle response based on return value
		if (response_time > 0.0)
		{
			// Successful reply - update statistics
			count_got_packet(&stats, response_time);
		}
		else if (response_time == ICMP_REPLY_TIMEOUT)
		{
			// No response received within timeout
			printf("Request timeout for icmp_seq=%d\n", ping_info.next_number);
		}
		// Note: ICMP error messages (ICMP_REPLY_ERROR) are already handled
		// and printed by handle_icmp_error() inside icmp_receive()
		
		if (g_stop)
			break;
		ping_info.next_number++;
		
		// Implementar delay según las opciones
		if (!args->flood_mode) {
			// Usar intervalo personalizado o 1 segundo por defecto
			if (args->interval >= 1.0) {
				sleep((unsigned int)args->interval);
			} else {
				// Para intervalos menores a 1 segundo, usar usleep
				usleep((useconds_t)(args->interval * 1000000));
			}
		}
		// En modo flood (-f), no hay delay entre paquetes
	}
	print_final_stats(&stats, args->target);
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
	start_ping_loop(socket_fd, &target_addr, &args);

	close_socket(socket_fd);
	cleanup_args(&args);
	return 0;
}
