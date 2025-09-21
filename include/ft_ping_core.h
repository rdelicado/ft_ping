/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_core.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/21 00:00:00 by rdelicado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_CORE_H
#define FT_PING_CORE_H

#include "ft_ping_types.h"
#include <stdbool.h>  /* Para bool */

/* ===== FUNCIONES PRINCIPALES ===== */

/* Core functions */
void	print_help(void);
int		main(int ac, char **av);
void	start_ping_loop(int socket_fd, struct sockaddr_in *target_addr, t_args *args);

/* Parsing functions */
void	parse_arguments(int ac, char **av, t_args *args);
void	validate_destination(t_args *args);
bool	handle_special_decimal(const char *input, char *out, size_t outlen);

/* Network functions */
int		create_socket(int sockfd);
void	close_socket(int sockfd);
int		check_if_ip(char *target, struct in_addr *out_addr);
int		find_hostname_ip(char *hostname, struct in_addr *out_addr);
int		find_target_address(const char *target, struct sockaddr_in *addr);

/* ICMP functions */
int		icmp_request(int sockfd, struct sockaddr_in *dest_addr, uint16_t *packet_id, uint16_t *sequence, int seq_counter);
double	icmp_receive(int sockfd, uint16_t expected_id, struct timeval *send_time, int mode_verbose);
uint16_t icmp_checksum(const void *buf, int len);

/* Statistics functions */
void	setup_stats(t_ping_stats *stats);
void	count_sent_packet(t_ping_stats *stats);
void	count_got_packet(t_ping_stats *stats, double response_time);
void	print_final_stats(t_ping_stats *stats, const char *target);

/* Signal functions */
void	setup_signal_handler(void);

#endif 