/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:06:23 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 18:39:16 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAIN_H
#define MAIN_H

# include "type.h"
# include "parse.h"
# include "signals.h"
# include "net.h"
# include "icmp.h"

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stddef.h>
# include <errno.h>
# include <ctype.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>

void	print_help(void);


#endif
