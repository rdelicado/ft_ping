/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_destination.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:12 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/14 11:57:28 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static bool	is_destination(char *dest)
{
	struct in_addr	addr;
	int i, dots;
	
	if (dest == NULL || dest[0] == '\0')
		return false;

	/* Manejar formato hexadecimal (ej. "0x3" -> "0.0.0.3") */
	if (dest[0] == '0' && dest[1] == 'x') {
		char *endptr;
		unsigned long ip = strtoul(dest + 2, &endptr, 16);
		if (endptr == dest + 2) return false; /* No dígitos después de 0x */
		if (*endptr == '\0' && ip <= 0xFFFFFFFF) {
			/* Convertir a formato IP decimal */
			sprintf(dest, "%lu.%lu.%lu.%lu",
				(ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
			/* Ahora validar como IP normal */
			return inet_pton(AF_INET, dest, &addr) == 1;
		}
		return false; /* Hex inválido */
	}
	
	/* Verificar que solo contenga dígitos y puntos */
	dots = 0;
	for (i = 0; dest[i]; i++) {
		if (dest[i] == '.') {
			dots++;
		} else if (dest[i] < '0' || dest[i] > '9') {
			return false; /* Contiene caracteres no válidos para IP */
		}
	}
	
	/* Una IP válida debe tener exactamente 3 puntos */
	if (dots != 3)
		return false;
	
	/* Usar inet_pton para validación final */
	if (inet_pton(AF_INET, dest, &addr) == 1) {
		return true;
	}
	return false;
}

static bool is_hostname(char *hostname)
{
	struct addrinfo hints, *res;
	int i, dots;
	
	if (hostname == NULL || hostname[0] == '\0')
		return false;
	
	/* Si es solo números, no es un hostname válido */
	bool all_digits = true;
	dots = 0;
	for (i = 0; hostname[i]; i++) {
		if (hostname[i] == '.') {
			dots++;
		} else if (hostname[i] < '0' || hostname[i] > '9') {
			all_digits = false;
		}
	}
	
	/* Si es solo números, no es un hostname válido */
	if (all_digits)
		return false;
	
	/* Si parece una IP incompleta (tiene puntos pero no 3), tratarla como IP decimal si es posible */
	if (dots > 0 && dots != 3) {
		/* Si solo tiene números y puntos, dejar que inet_pton lo maneje */
		bool only_digits_and_dots = true;
		for (i = 0; hostname[i]; i++) {
			if (hostname[i] != '.' && (hostname[i] < '0' || hostname[i] > '9')) {
				only_digits_and_dots = false;
				break;
			}
		}
		if (only_digits_and_dots)
			return false;  // Dejar que lo maneje is_destination o conversión decimal
	}
		
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // Solo IPv4
	hints.ai_socktype = 0; // No forzar tipo
	hints.ai_protocol = 0;
	
	if (getaddrinfo(hostname, NULL, &hints, &res) == 0) {
		freeaddrinfo(res);
		return true;
	}
	return false;
}

void	validate_destination(t_args *args)
{
	if (args == NULL || args->dest == NULL || args->dest[0] == '\0') {
		printf("ft_ping: : No address associated with hostname\n");
		exit(2);
	}
	
	// Verificar dirección broadcast (255.255.255.255) - igual que ping original
	if (strcmp(args->dest, "255.255.255.255") == 0) {
		printf("ft_ping: Do you want to ping broadcast? Then -b. If not, check your local firewall rules\n");
		exit(2);
	}
	
	// Verificar longitud excesiva
	if (strlen(args->dest) > 100) {
		printf("ft_ping: %s: Name or service not known\n", args->dest);
		exit(2);
	}
			
	if (!is_destination(args->dest) && !is_hostname(args->dest)) {
		bool has_invalid_chars = false;
		int dots = 0;
		bool only_digits_dots = true;
		for (int i = 0; args->dest[i]; i++) {
			char c = args->dest[i];
			if (c == '.') dots++;
			else if (!isdigit((unsigned char)c)) only_digits_dots = false;
			if (!isalnum((unsigned char)c) && c != '.') {
				has_invalid_chars = true;
				break;
			}
		}
		if (has_invalid_chars || (dots == 3 && only_digits_dots) || (dots == 3 && !only_digits_dots) || dots > 3 || (dots > 0 && dots < 3 && !only_digits_dots)) {
			printf("ft_ping: %s: Name or service not known\n", args->dest);
		} else {
			printf("ft_ping: %s: Temporary failure in name resolution\n", args->dest);
		}
		exit(2);
	}
}