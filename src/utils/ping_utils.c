/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/10/24 17:15:30 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	send_ping_packet(t_ping_context *ping_info)
{
	if (icmp_request(ping_info->socket_fd, ping_info->target_addr, &ping_info->packet_id, &ping_info->packet_number, ping_info->next_number, ping_info->args->packet_bytes) == 0) {
		count_sent_packet(ping_info->stats);
	} else {
		printf("Error enviando paquete icmp_seq=%d\n", ping_info->next_number);
	}
}