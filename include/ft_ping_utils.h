/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/21 00:00:00 by rdelicado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_UTILS_H
#define FT_PING_UTILS_H

#include "ft_ping_types.h"

/* ===== FUNCIONES DE UTILIDAD ===== */

/* Cleanup functions */
void	cleanup_args(t_args *args);

/* Resolver utilities */
void	convert_ip_binary(int socket_fd, t_args *args, struct sockaddr_in *target_addr);

/* Ping utilities */
void	handle_ping_response(t_ping_context *ping_info, double response_time);
void	send_ping_packet(t_ping_context *ping_info);
int		create_and_validate_socket(t_args *args);

/* Verbose functions */
void	verbose_socket_info(int mode_verbose, int socket_fd);
void	verbose_resolution_info(int mode_verbose, const char *target, struct sockaddr_in *addr);
void	verbose_resolution_error(int mode_verbose, const char *target, const char *error_msg);

#endif