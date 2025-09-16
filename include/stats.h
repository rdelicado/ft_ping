/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:29:15 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:50:17 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATS_H
# define STATS_H

# include "type.h"

/* Initialization and cleanup */
void	init_stats(t_ping_stats *stats);

/* Statistics update */
void	update_stats_packet_sent(t_ping_stats *stats);
void	update_stats_packet_received(t_ping_stats *stats, double rtt);

/* Statistics calculations */
double	calculate_packet_loss(t_ping_stats *stats);
double	calculate_avg_rtt(t_ping_stats *stats);
double	calculate_stddev_rtt(t_ping_stats *stats);
long	calculate_total_time(t_ping_stats *stats);

/* Display statistics */
void	print_final_stats(t_ping_stats *stats, const char *dest);

#endif
