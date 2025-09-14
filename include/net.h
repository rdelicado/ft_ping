/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:12:37 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/14 14:32:05 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NET_H
#define NET_H

#include "ft_ping.h"

int		create_socket(int sockfd);
void	close_socket(int sockfd);

#endif
