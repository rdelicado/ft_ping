/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:56:39 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/14 14:04:14 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <signal.h>

volatile sig_atomic_t g_stop = 0;

static void signal_handler(int sig)
{
    if (sig == SIGINT) {
        printf("  Exiting...\n");
        g_stop = 1;
    }
}

void    setup_signal_handler()
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        printf("Error setting up signal handler.\n");
        exit(EXIT_FAILURE);
    }
    
}