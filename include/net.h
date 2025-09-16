/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:12:37 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 16:36:34 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NET_H
#define NET_H

#include "ft_ping.h"
#include <netdb.h>
#include <sys/time.h>

int			create_socket(int sockfd);
void		close_socket(int sockfd);
int			resolve_ip(char *dest, struct in_addr *out_addr);
int			resolve_hostname(char *hostname, struct in_addr *out_addr);
int			resolve_destination(const char *dest, struct sockaddr_in *addr);
uint16_t	icmp_checksum(const void *buf, int len);
double		icmp_receive(int sockfd, uint16_t id, struct timeval *send_time, int verbose);
#endif
