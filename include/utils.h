/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:14:20 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:25:18 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "ft_ping.h"

void	verbose_socket_info(int verbose, int sockfd);
void	verbose_resolution_info(int verbose, const char *dest, struct sockaddr_in *addr);
void	verbose_resolution_error(int verbose, const char *dest, const char *error_msg);

#endif
