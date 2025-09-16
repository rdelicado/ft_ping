/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:29:15 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 09:38:23 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATS_H
# define STATS_H

# include <sys/time.h>
# include <stdint.h>

typedef struct s_ping_stats
{
	int			packets_transmitted;
	int			packets_received;
	double		min_rtt;
	double		max_rtt;
	double		sum_rtt;
	double		sum_rtt_squared;
	struct timeval	start_time;
	struct timeval	end_time;
}	t_ping_stats;

/* Inicialización y limpieza */
void	init_stats(t_ping_stats *stats);

/* Actualización de estadísticas */
void	update_stats_packet_sent(t_ping_stats *stats);
void	update_stats_packet_received(t_ping_stats *stats, double rtt);

/* Cálculos de estadísticas */
double	calculate_packet_loss(t_ping_stats *stats);
double	calculate_avg_rtt(t_ping_stats *stats);
double	calculate_stddev_rtt(t_ping_stats *stats);
long	calculate_total_time(t_ping_stats *stats);

/* Mostrar estadísticas */
void	print_final_stats(t_ping_stats *stats, const char *dest);

#endif
