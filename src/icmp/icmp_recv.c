/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_recv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:38:36 by rdelicad          #+#    #+#             */
/*   Updated: 2025/10/24 17:11:00 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <errno.h>

double	icmp_receive(int sockfd, uint16_t id, struct timeval *send_time, int verbose)
{
	char				buffer[1024];
	struct sockaddr_in	addr;
	socklen_t			addrlen = sizeof(addr);
	ssize_t				len;
	struct iphdr		*ip;
	struct icmphdr		*icmp;
	struct timeval		recv_time;
	struct timeval		timeout;
	int					attempts = 0;
	const int			max_attempts = 3;

	// Configure longer timeout for localhost and fast networks
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000; // 500ms
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	// Intentar recibir hasta 3 veces para manejar paquetes fuera de orden
	while (attempts < max_attempts) {
		// Paso 1: Esperar que llegue un paquete
		len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr *)&addr, &addrlen);

		// Paso 2: Verificar que no hubo error
		if (len < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				attempts++;
				continue; // Intentar de nuevo
			}
			perror("recvfrom");
			return -1.0;
		}

		// Step 3: Mark when it arrived
		gettimeofday(&recv_time, NULL);

		// Paso 4: Interpretar el paquete como cabecera IP
		ip = (struct iphdr *)buffer;

		// Paso 5: Encontrar la parte ICMP dentro del paquete
		icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));

		// Step 6: Check ICMP message type
		if (icmp->type == ICMP_ECHOREPLY && icmp->un.echo.id == id) {
			// Step 7: Calculate how long it took
			double rtt = (recv_time.tv_sec - send_time->tv_sec) * 1000.0 +
							(recv_time.tv_usec - send_time->tv_usec) / 1000.0;

			// Paso 8: Mostrar la respuesta
			if (verbose) {
				printf("64 bytes from %s: icmp_seq=%d ident=%d ttl=%d time=%.3f ms\n",
					inet_ntoa(addr.sin_addr),
					icmp->un.echo.sequence,
					id,
					ip->ttl,
					rtt);
			} else {
				printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
					inet_ntoa(addr.sin_addr),
					icmp->un.echo.sequence,
					ip->ttl,
					rtt);
			}
			
			return rtt; // ¡Devolver el RTT calculado!
		}
		// Handle ICMP error messages
		else if (icmp->type == ICMP_DEST_UNREACH || 
				 icmp->type == ICMP_TIME_EXCEEDED ||
				 icmp->type == ICMP_REDIRECT ||
				 icmp->type == ICMP_PARAMETERPROB ||
				 icmp->type == ICMP_SOURCE_QUENCH) {
			// Extract original packet info from ICMP error payload
			struct iphdr *orig_ip = (struct iphdr *)(icmp + 1);
			struct icmphdr *orig_icmp = (struct icmphdr *)((char *)orig_ip + (orig_ip->ihl * 4));
			
			// Verify it's our packet
			if (orig_icmp->un.echo.id == id) {
				handle_icmp_error(icmp, &addr, orig_icmp->un.echo.sequence);
				return -2.0; // Return special value for ICMP errors
			}
		}
		
		// Si no es para nosotros, intentar de nuevo
		attempts++;
	}

	return -1.0; // Timeout after several attempts
}
