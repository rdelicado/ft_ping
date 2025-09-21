/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolver_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/21 00:00:00 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	convert_ip_binary(int socket_fd, t_args *args, struct sockaddr_in *target_addr)
{
	// Imprimir información del socket si está en modo verbose
	verbose_socket_info(args->mode_verbose, socket_fd);
	
	// Resolver el destino a IP binaria
	if (find_target_address(args->target, target_addr) == 0) {
		const char *error_msg = "Name or service not known";
		if (args->mode_verbose)
			verbose_resolution_error(args->mode_verbose, args->target, error_msg);
		else
			printf("ft_ping: %s: %s\n", args->target, error_msg);
		close_socket(socket_fd);
		cleanup_args(args);
		exit(2);
	}
	verbose_resolution_info(args->mode_verbose, args->target, target_addr);
	printf("PING %s (%s) 56(84) bytes of data.\n", args->target, inet_ntoa(target_addr->sin_addr));
}