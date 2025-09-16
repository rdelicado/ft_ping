/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:37:35 by rdelicad         ###   ########.fr       */
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

void	convert_ip_binary(int sockfd, t_args *args, struct sockaddr_in *dest_addr)
{
	// Resolver el destino a IP binaria
	if (resolve_destination(args->dest, dest_addr) == 0) {
		const char *error_msg = "Name or service not known";
		if (args->verbose)
			verbose_resolution_error(args->verbose, args->dest, error_msg);
		else
			printf("ft_ping: %s: %s\n", args->dest, error_msg);
		close_socket(sockfd);
		cleanup_args(args);
		exit(2);
	}
	verbose_resolution_info(args->verbose, args->dest, dest_addr);
	printf("PING %s (%s) 56(84) bytes of data.\n", args->dest, inet_ntoa(dest_addr->sin_addr));
}

static void	handle_ping_response(double rtt_ms, int seq_counter, t_ping_stats *stats)
{
	if (rtt_ms >= 0.0) {
		update_stats_packet_received(stats, rtt_ms);
	} else {
		printf("Request timeout for icmp_seq=%d\n", seq_counter);
	}
}

static void	send_ping_packet(int sockfd, struct sockaddr_in *dest_addr,
					uint16_t *packet_id, uint16_t *sequence, int seq_counter,
					t_ping_stats *stats)
{
	if (icmp_request(sockfd, dest_addr, packet_id, sequence, seq_counter) == 0) {
		update_stats_packet_sent(stats);
	} else {
		printf("Error enviando paquete icmp_seq=%d\n", seq_counter);
	}
}

void	ping_loop(int sockfd, struct sockaddr_in *dest_addr, char *dest, int verbose)
{
	struct timeval	send_time;
	uint16_t		packet_id;
	uint16_t		sequence;
	int				seq_counter;
	t_ping_stats	stats;
	double			rtt_ms;

	init_stats(&stats);
	seq_counter = 1;
	while (!g_stop)
	{
		gettimeofday(&send_time, NULL);
		send_ping_packet(sockfd, dest_addr, &packet_id, &sequence, seq_counter, &stats);
		rtt_ms = icmp_receive(sockfd, packet_id, &send_time, verbose);
		handle_ping_response(rtt_ms, seq_counter, &stats);
		if (g_stop)
			break;
		seq_counter++;
		sleep(1);
	}
	print_final_stats(&stats, dest);
}

static int	setup_program(t_args *args, int ac, char **av)
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

static int	create_and_validate_socket(void)
{
	int sockfd = create_socket(0);
	if (sockfd < 0) {
		perror("ft_ping: socket");
		return -1;
	}
	return sockfd;
}

int	main(int ac, char **av)
{
	struct sockaddr_in	dest_addr;
	t_args				args;
	int					sockfd;

	if (setup_program(&args, ac, av) != 0)
		return 1;

	sockfd = create_and_validate_socket();
	if (sockfd < 0) {
		cleanup_args(&args);
		return 1;
	}

	convert_ip_binary(sockfd, &args, &dest_addr);
	ping_loop(sockfd, &dest_addr, args.dest, args.verbose);

	close_socket(sockfd);
	cleanup_args(&args);
	return 0;
}
