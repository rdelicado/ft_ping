/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:11:35 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 16:52:53 by rdelicad         ###   ########.fr       */
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
