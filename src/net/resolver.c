/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 15:02:55 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 09:53:30 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int	resolve_ip(char *dest, struct in_addr *out_addr)
{
	struct in_addr	addr;
	int i, dots;
	
	if (dest == NULL || dest[0] == '\0')
		return 0;

	/* Manejar formato hexadecimal (ej. "0x3" -> "0.0.0.3") */
	if (dest[0] == '0' && dest[1] == 'x') {
		char *endptr;
		unsigned long ip = strtoul(dest + 2, &endptr, 16);
		if (endptr == dest + 2) return 0; /* No dígitos después de 0x */
		if (*endptr == '\0' && ip <= 0xFFFFFFFF) {
			/* Convertir a formato IP decimal */
			sprintf(dest, "%lu.%lu.%lu.%lu",
				(ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
			/* Ahora validar como IP normal y guardar IP binaria */
			if (inet_pton(AF_INET, dest, &addr) == 1) {
				if (out_addr) *out_addr = addr;
				return 1;
			}
		}
		return 0; /* Hex inválido */
	}
	
	/* Verificar que solo contenga dígitos y puntos */
	dots = 0;
	for (i = 0; dest[i]; i++) {
		if (dest[i] == '.') {
			dots++;
		} else if (dest[i] < '0' || dest[i] > '9') {
			return 0; /* Contiene caracteres no válidos para IP */
		}
	}
	
	/* Una IP válida debe tener exactamente 3 puntos */
	if (dots != 3)
		return 0;
	
	/* Usar inet_pton para validación final y guardar IP binaria */
	if (inet_pton(AF_INET, dest, &addr) == 1) {
		if (out_addr) *out_addr = addr;
		return 1;
	}
	return 0;
}

int	resolve_hostname(char *hostname, struct in_addr *out_addr)
{
	struct addrinfo hints, *res;
	int i, dots;
	
	if (hostname == NULL || hostname[0] == '\0')
		return 0;
	
	/* Si es solo números, no es un hostname válido */
	int all_digits = 1;
	dots = 0;
	for (i = 0; hostname[i]; i++) {
		if (hostname[i] == '.') {
			dots++;
		} else if (hostname[i] < '0' || hostname[i] > '9') {
			all_digits = 0;
		}
	}
	
	/* Si es solo números, no es un hostname válido */
	if (all_digits)
		return 0;
	
	/* Si parece una IP incompleta (tiene puntos pero no 3), tratarla como IP decimal si es posible */
	if (dots > 0 && dots != 3) {
		/* Si solo tiene números y puntos, dejar que inet_pton lo maneje */
		int only_digits_and_dots = 1;
		for (i = 0; hostname[i]; i++) {
			if (hostname[i] != '.' && (hostname[i] < '0' || hostname[i] > '9')) {
				only_digits_and_dots = 0;
				break;
			}
		}
		if (only_digits_and_dots)
			return 0;  // Dejar que lo maneje is_destination o conversión decimal
	}
		
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // Solo IPv4
	hints.ai_socktype = 0; // No forzar tipo
	hints.ai_protocol = 0;
	
	if (getaddrinfo(hostname, NULL, &hints, &res) == 0) {
		struct sockaddr_in *sin = (struct sockaddr_in *)res->ai_addr;
		if (out_addr) *out_addr = sin->sin_addr; // Guardar IP binaria
		freeaddrinfo(res);
		return 1;
	}
	return 0;
}

int resolve_destination(const char *dest, struct sockaddr_in *addr)
{
	char dest_copy[256];
	struct in_addr ip_addr;

	if (!dest || !addr)
		return 0;

	// Hacer copia para evitar modificar el original
	strncpy(dest_copy, dest, sizeof(dest_copy) - 1);
	dest_copy[sizeof(dest_copy) - 1] = '\0';

	// Inicializar sockaddr_in
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_addr = ip_addr;

	// Resolver IP
	if (resolve_ip(dest_copy, &ip_addr)) {
		addr->sin_addr = ip_addr;
		return 1;
	}

	// Si no es IP, resovler hostname
	if (resolve_hostname(dest_copy, &ip_addr)) {
		addr->sin_addr = ip_addr;
		return 1;
	}

	return 0;
}