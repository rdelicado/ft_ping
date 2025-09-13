/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_destination.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:12 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/13 13:24:48 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static bool	is_destination(char *dest)
{
	struct in_addr	addr;
	int i, dots;
	
	if (dest == NULL || dest[0] == '\0')
		return false;
	
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
	
	/* Si es solo números o si parece una IP incompleta (tiene puntos pero no 3), no es hostname válido */
	if (all_digits || (dots > 0 && dots != 3))
		return false;
		
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
		printf("Error: falta destino.\n");
		return ;
	}
	
	printf("flag: %s\n", args->flag ? args->flag : "NULL");
	printf("destination: %s\n", args->dest);

	if (!is_destination(args->dest) && !is_hostname(args->dest)) {
		printf("Error: [%s] No es una destino valido.\n", args->dest);
	}
}