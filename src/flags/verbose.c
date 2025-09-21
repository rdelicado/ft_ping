/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:05:17 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:50:17 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static const char *family_to_str(int f)
{
	if (f == AF_INET) return "AF_INET";
	return "UNKNOWN_FAMILY";
}

static const char *socktype_to_str(int t)
{
	if (t == SOCK_RAW) return "SOCK_RAW";
	if (t == SOCK_DGRAM) return "SOCK_DGRAM";
	if (t == SOCK_STREAM) return "SOCK_STREAM";
	return "SOCK_?";
}

void	verbose_socket_info(int mode_verbose, int socket_fd)
{
	int	stype = 0;
	socklen_t len;

	if (!mode_verbose)
		return ;

	// Obtener socktype real
	len = sizeof(stype);
	if (getsockopt(socket_fd, SOL_SOCKET, SO_TYPE, &stype, &len) < 0)
		stype = SOCK_RAW; // Fallback porque sabemos que es RAW

	// Imprimir en el formato esperado por ping estándar
	// Simular IPv4 y IPv6 como hace ping real, aunque solo usemos IPv4
	printf("ft_ping: sock4.fd: %d (socktype: %s), hints.ai_family: AF_INET\n\n",
		socket_fd, socktype_to_str(stype));
}

void	verbose_resolution_info(int mode_verbose, const char *target, struct sockaddr_in *addr)
{
	int is_ip;

	if (!mode_verbose || !target || !addr)
		return ;
	is_ip = 1;
	for (int i = 0; target[i]; i++) {
		if ((target[i] < '0' || target[i] > '9') && target[i] != '.' && target[i] != 'x' && target[i] != 'X') {
			is_ip = 0;
			break;
		}
	}
	if (is_ip)
		printf("ai->ai_family: %s, ai->ai_canonname: NULL (direct IP)\n", family_to_str(AF_INET));
	else
		printf("ai->ai_family: %s, ai->ai_canonname: '%s'\n", family_to_str(AF_INET), target);
}



void	verbose_resolution_error(int mode_verbose, const char *target, const char *error_msg)
{
	if (!mode_verbose || !target || !error_msg)
		return ;
	printf("ft_ping: attempting to resolve hostname '%s' (ai_family: %s)\n", target, family_to_str(AF_INET));
	printf("ft_ping: %s: %s\n", target, error_msg);
}
