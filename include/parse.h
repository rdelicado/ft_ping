/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:23:22 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/12 10:57:58 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSE_H
#define PARSE_H

#include "ft_ping.h"

void	print_error(const char *msg);
void	parse_arguments(int ac, char **av, t_args *args);
void	validate_destination(t_args *args);

#endif
