/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:11:35 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/23 11:41:53 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	close_socket(int sockfd)
{
	if (sockfd >= 0) {
		close(sockfd);
	}
}

int	create_socket(int sockfd)
{
	// socket creado para capa 3 (red) modelo OSI
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		return -1;
	}

	return sockfd;
}

int	set_socket_ttl(int sockfd, int ttl)
{
	if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
		perror("setsockopt TTL");
		return -1;
	}
	return 0;
}
