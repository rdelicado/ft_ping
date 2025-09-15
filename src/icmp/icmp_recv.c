/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_recv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:38:36 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 19:10:29 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <errno.h>

int	icmp_receive(int sockfd, uint16_t id, struct timeval *send_time)
{
	char				buffer[1024];
	struct sockaddr_in	addr;
	socklen_t			addrlen = sizeof(addr);
	ssize_t				len;
	struct iphdr		*ip;
	struct icmphdr		*icmp;
	struct timeval		recv_time;
	struct timeval		timeout;

	// Configurar timeout de 1 segundo para evitar bloqueo
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	// Paso 1: Esperar que llegue un paquete
	len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
		(struct sockaddr *)&addr, &addrlen);

	// Paso 2: Verificar que no hubo error
	if (len < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return -1; // Timeout, sin mensaje de error
		}
		perror("recvfrom");
		return -1;
	}

	// Paso 3: Marcar cuándo llegó
	gettimeofday(&recv_time, NULL);

	// Paso 4: Interpretar el paquete como cabecera IP
	ip = (struct iphdr *)buffer;

	// Paso 5: Encontrar la parte ICMP dentro del paquete
	icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));

	// Paso 6: ¿Es la respuesta que esperamos?
	if (icmp->type == ICMP_ECHOREPLY && icmp->un.echo.id == id) {
		// Paso 7: Calcular cuánto tardó
		double rtt = (recv_time.tv_sec - send_time->tv_sec) * 1000.0 +
						(recv_time.tv_usec - send_time->tv_usec) / 1000.0;

		// Paso 8: Mostrar la respuesta
		printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
			inet_ntoa(addr.sin_addr),
			icmp->un.echo.sequence,
			ip->ttl,
			rtt);
		
		return 0; // ¡Éxito!
	}

	return -1; // No era para nosotros
}
