/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/10/24 17:11:00 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

/**
 * handle_dest_unreach - Maneja mensajes ICMP Destination Unreachable (Type 3)
 * @code: Código específico del error
 * @source_ip: IP de origen del mensaje de error
 * @seq: Número de secuencia del paquete original
 */
static void	handle_dest_unreach(uint8_t code, const char *source_ip, int seq)
{
	printf("From %s icmp_seq=%d ", source_ip, seq);
	
	if (code == ICMP_NET_UNREACH)
		printf("Destination Net Unreachable\n");
	else if (code == ICMP_HOST_UNREACH)
		printf("Destination Host Unreachable\n");
	else if (code == ICMP_PROT_UNREACH)
		printf("Destination Protocol Unreachable\n");
	else if (code == ICMP_PORT_UNREACH)
		printf("Destination Port Unreachable\n");
	else if (code == ICMP_FRAG_NEEDED)
		printf("Fragmentation Needed and DF set\n");
	else if (code == ICMP_SR_FAILED)
		printf("Source Route Failed\n");
	else if (code == ICMP_NET_UNKNOWN)
		printf("Destination Network Unknown\n");
	else if (code == ICMP_HOST_UNKNOWN)
		printf("Destination Host Unknown\n");
	else
		printf("Destination Unreachable (code %d)\n", code);
}

/**
 * handle_time_exceeded - Maneja mensajes ICMP Time Exceeded (Type 11)
 * @code: Código específico del error
 * @source_ip: IP de origen del mensaje de error
 * @seq: Número de secuencia del paquete original
 */
static void	handle_time_exceeded(uint8_t code, const char *source_ip, int seq)
{
	printf("From %s icmp_seq=%d ", source_ip, seq);
	
	if (code == ICMP_EXC_TTL)
		printf("Time to live exceeded\n");
	else if (code == ICMP_EXC_FRAGTIME)
		printf("Fragment reassembly time exceeded\n");
	else
		printf("Time Exceeded (code %d)\n", code);
}

/**
 * handle_icmp_error - Procesa y muestra mensajes de error ICMP
 * @icmp: Puntero a la cabecera ICMP del mensaje de error
 * @addr: Dirección del host que envió el error
 * @seq: Número de secuencia del paquete original
 * 
 * Esta función identifica el tipo de error ICMP y muestra el mensaje
 * apropiado según el tipo y código del error.
 */
void	handle_icmp_error(struct icmphdr *icmp, struct sockaddr_in *addr, int seq)
{
	char	*source_ip;

	source_ip = inet_ntoa(addr->sin_addr);
	
	if (icmp->type == ICMP_DEST_UNREACH)
		handle_dest_unreach(icmp->code, source_ip, seq);
	else if (icmp->type == ICMP_TIME_EXCEEDED)
		handle_time_exceeded(icmp->code, source_ip, seq);
	else if (icmp->type == ICMP_REDIRECT)
	{
		printf("From %s icmp_seq=%d ", source_ip, seq);
		printf("Redirect (code %d)\n", icmp->code);
	}
	else if (icmp->type == ICMP_PARAMETERPROB)
	{
		printf("From %s icmp_seq=%d ", source_ip, seq);
		printf("Parameter Problem\n");
	}
	else if (icmp->type == ICMP_SOURCE_QUENCH)
	{
		printf("From %s icmp_seq=%d ", source_ip, seq);
		printf("Source Quench\n");
	}
}
