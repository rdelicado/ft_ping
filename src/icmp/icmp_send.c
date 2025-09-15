/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:52:49 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 19:07:07 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int icmp_request(int sockfd, struct sockaddr_in *dest_addr, uint16_t *id, uint16_t *sequence, int seq_counter)
{
	char	packet[64];
	struct icmphdr *icmp;

	icmp = (struct icmphdr *)packet;
	memset(packet, 0, sizeof(packet));
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xFFFF;
	icmp->un.echo.sequence = seq_counter;

	// Guardar los valores para devolverlos
	*id = icmp->un.echo.id;
	*sequence = icmp->un.echo.sequence;

	const char *data = "ft_ping payload";
	size_t data_len = strlen(data);
	memcpy(packet + sizeof(struct icmphdr), data, data_len);

	int packet_size = sizeof(struct icmphdr) + data_len;
	icmp->checksum = icmp_checksum(icmp, packet_size);

	ssize_t sent = sendto(sockfd, packet, packet_size, 0,
		(struct sockaddr *)dest_addr, sizeof(*dest_addr));
	if (sent < 0) {
		perror("sendto");
		return -1;
	}
	
	return 0;
}