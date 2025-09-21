/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_destination.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:39:12 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:50:17 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	validate_destination(t_args *args)
{
	if (args == NULL || args->target == NULL || args->target[0] == '\0') {
		printf("ft_ping: : No address associated with hostname\n");
		exit(2);
	}
	
	// Check broadcast address (255.255.255.255) - same as original ping
	if (strcmp(args->target, "255.255.255.255") == 0) {
		printf("ft_ping: Do you want to ping broadcast? Then -b. If not, check your local firewall rules\n");
		exit(2);
	}
	
	// Verificar longitud excesiva
	if (strlen(args->target) > 100) {
		printf("ft_ping: %s: Name or service not known\n", args->target);
		exit(2);
	}
			
	if (!check_if_ip(args->target, NULL) && !find_hostname_ip(args->target, NULL)) {
		bool has_invalid_chars = false;
		int dots = 0;
		bool only_digits_dots = true;
		for (int i = 0; args->target[i]; i++) {
			char c = args->target[i];
			if (c == '.') dots++;
			else if (!isdigit((unsigned char)c)) only_digits_dots = false;
			if (!isalnum((unsigned char)c) && c != '.') {
				has_invalid_chars = true;
				break;
			}
		}
		if (has_invalid_chars || (dots == 3 && only_digits_dots) || (dots == 3 && !only_digits_dots) || dots > 3 || (dots > 0 && dots < 3 && !only_digits_dots)) {
			printf("ft_ping: %s: Name or service not known\n", args->target);
		} else {
			printf("ft_ping: %s: Temporary failure in name resolution\n", args->target);
		}
		exit(2);
	}
}