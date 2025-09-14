/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:06:23 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/14 15:03:30 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAIN_H
#define MAIN_H

# include "type.h"
# include "parse.h"
# include "signals.h"
# include "net.h"

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stddef.h>
# include <ctype.h>
# include <signal.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>

void	print_help(void);


#endif
