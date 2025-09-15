/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:53:31 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 19:07:07 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
#define ICMP_H

#include "ft_ping.h"
#include <netinet/ip_icmp.h>

int		icmp_request(int sockfd, struct sockaddr_in *dest_addr, uint16_t *id, uint16_t *sequence, int seq_counter);

#endif
