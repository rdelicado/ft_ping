/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:58:39 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 19:10:29 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
#define SIGNAL_H

#include "ft_ping.h"
#include <signal.h>

extern volatile sig_atomic_t g_stop;

void	setup_signal_handler();

#endif
