/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/23 11:41:53 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int	create_and_validate_socket(t_args *args)
{
	int sockfd = create_socket(0);
	if (sockfd < 0) {
		perror("ft_ping: socket");
		cleanup_args(args);
		return -1;
	}
	
	// Configurar TTL si es diferente del valor por defecto
	if (args->time_to_live != 64) {
		if (set_socket_ttl(sockfd, args->time_to_live) < 0) {
			close_socket(sockfd);
			cleanup_args(args);
			return -1;
		}
	}
	
	return sockfd;
}