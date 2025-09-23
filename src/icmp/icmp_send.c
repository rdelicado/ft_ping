/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:52:49 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/23 11:41:53 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int icmp_request(int sockfd, struct sockaddr_in *dest_addr, uint16_t *id, uint16_t *sequence, int seq_counter, int packet_size)
{
	char	*packet;
	struct icmphdr *icmp;
	int		total_size;
	int		data_size;

	// Calcular el tamaño total del paquete (header ICMP + datos)
	total_size = sizeof(struct icmphdr) + packet_size;
	data_size = packet_size;

	// Asignar memoria dinámicamente para el paquete
	packet = malloc(total_size);
	if (!packet) {
		perror("malloc");
		return -1;
	}

	icmp = (struct icmphdr *)packet;
	memset(packet, 0, total_size);
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = getpid() & 0xFFFF;
	icmp->un.echo.sequence = seq_counter;

	// Guardar los valores para devolverlos
	*id = icmp->un.echo.id;
	*sequence = icmp->un.echo.sequence;

	// Llenar los datos del paquete
	char *data_ptr = packet + sizeof(struct icmphdr);
	const char *pattern = "ft_ping payload data ";
	int pattern_len = strlen(pattern);
	
	for (int i = 0; i < data_size; i++) {
		data_ptr[i] = pattern[i % pattern_len];
	}

	icmp->checksum = icmp_checksum(icmp, total_size);

	ssize_t sent = sendto(sockfd, packet, total_size, 0,
		(struct sockaddr *)dest_addr, sizeof(*dest_addr));
	
	free(packet);
	
	if (sent < 0) {
		perror("sendto");
		return -1;
	}
	
	return 0;
}