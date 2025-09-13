/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:23:22 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/13 16:31:47 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSE_H
#define PARSE_H

# include "ft_ping.h"
# include <stdbool.h>
# include <stddef.h>

void	print_error(const char *msg);
void	parse_arguments(int ac, char **av, t_args *args);
void	validate_destination(t_args *args);
bool	is_decimal_format(const char *ip);
bool	decimal_to_ip(const char *dec, char *out, size_t outlen);
bool	handle_special_decimal(const char *input, char *out, size_t outlen);

#endif
