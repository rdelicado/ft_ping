/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:29:58 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 09:41:55 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stats.h"
#include "ft_ping.h"
#include <math.h>
#include <float.h>

void	init_stats(t_ping_stats *stats)
{
	if (!stats)
		return ;
	stats->packets_transmitted = 0;
	stats->packets_received = 0;
	stats->min_rtt = DBL_MAX;
	stats->max_rtt = 0.0;
	stats->sum_rtt = 0.0;
	stats->sum_rtt_squared = 0.0;
	gettimeofday(&stats->start_time, NULL);
}

void	update_stats_packet_sent(t_ping_stats *stats)
{
	if (!stats)
		return ;
	stats->packets_transmitted++;
}

void	update_stats_packet_received(t_ping_stats *stats, double rtt)
{
	if (!stats)
		return ;
	stats->packets_received++;
	stats->sum_rtt += rtt;
	stats->sum_rtt_squared += (rtt * rtt);
	if (rtt < stats->min_rtt)
		stats->min_rtt = rtt;
	if (rtt > stats->max_rtt)
		stats->max_rtt = rtt;
}

double	calculate_packet_loss(t_ping_stats *stats)
{
	if (!stats || stats->packets_transmitted == 0)
		return (100.0);
	return ((double)(stats->packets_transmitted - stats->packets_received)
		/ stats->packets_transmitted * 100.0);
}

double	calculate_avg_rtt(t_ping_stats *stats)
{
	if (!stats || stats->packets_received == 0)
		return (0.0);
	return (stats->sum_rtt / stats->packets_received);
}

double	calculate_stddev_rtt(t_ping_stats *stats)
{
	double	avg;
	double	variance;

	if (!stats || stats->packets_received <= 1)
		return (0.0);
	avg = calculate_avg_rtt(stats);
	variance = (stats->sum_rtt_squared / stats->packets_received)
		- (avg * avg);
	if (variance < 0.0)
		variance = 0.0;
	return (sqrt(variance));
}

long	calculate_total_time(t_ping_stats *stats)
{
	struct timeval	current_time;
	long		total_ms;

	if (!stats)
		return (0);
	gettimeofday(&current_time, NULL);
	total_ms = (current_time.tv_sec - stats->start_time.tv_sec) * 1000;
	total_ms += (current_time.tv_usec - stats->start_time.tv_usec) / 1000;
	return (total_ms);
}

void	print_final_stats(t_ping_stats *stats, const char *dest)
{
	double	packet_loss;
	double	avg_rtt;
	double	stddev_rtt;
	long	total_time;

	if (!stats || !dest)
		return ;
	packet_loss = calculate_packet_loss(stats);
	avg_rtt = calculate_avg_rtt(stats);
	stddev_rtt = calculate_stddev_rtt(stats);
	total_time = calculate_total_time(stats);
	printf("\n--- %s ping statistics ---\n", dest);
	if (packet_loss == 0.0)
		printf("%d packets transmitted, %d received, 0%% packet loss",
			stats->packets_transmitted, stats->packets_received);
	else
		printf("%d packets transmitted, %d received, %.0f%% packet loss",
			stats->packets_transmitted, stats->packets_received, packet_loss);
	printf(", time %ldms\n", total_time);
	if (stats->packets_received > 0)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			stats->min_rtt, avg_rtt, stats->max_rtt, stddev_rtt);
}