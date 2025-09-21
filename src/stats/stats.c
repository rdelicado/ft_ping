/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:29:58 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:37:35 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	setup_stats(t_ping_stats *stats)
{
	if (!stats)
		return ;
	stats->packets_sent = 0;
	stats->packets_got = 0;
	stats->fastest_time = DBL_MAX;
	stats->slowest_time = 0.0;
	stats->total_time = 0.0;
	stats->total_time_squared = 0.0;
	gettimeofday(&stats->start_moment, NULL);
}

void	count_sent_packet(t_ping_stats *stats)
{
	if (!stats)
		return ;
	stats->packets_sent++;
}

void	count_got_packet(t_ping_stats *stats, double response_time)
{
	if (!stats)
		return ;

	stats->packets_got++;
	stats->total_time += response_time;
	stats->total_time_squared += (response_time * response_time);

	if (response_time < stats->fastest_time)
		stats->fastest_time = response_time;
	if (response_time > stats->slowest_time)
		stats->slowest_time = response_time;

	gettimeofday(&stats->end_moment, NULL);
}

double	get_packet_loss_percent(t_ping_stats *stats)
{
	if (!stats || stats->packets_sent == 0)
		return (100.0);
	return ((double)(stats->packets_sent - stats->packets_got)
		/ stats->packets_sent * 100.0);
}

double	get_average_time(t_ping_stats *stats)
{
	if (!stats || stats->packets_got == 0)
		return (0.0);
	return (stats->total_time / stats->packets_got);
}

double	get_time_variation(t_ping_stats *stats)
{
	double	avg;
	double	variance;

	if (!stats || stats->packets_got <= 1)
		return (0.0);
	avg = get_average_time(stats);
	variance = (stats->total_time_squared / stats->packets_got)
		- (avg * avg);
	if (variance < 0.0)
		variance = 0.0;
	return (sqrt(variance));
}

long	get_total_run_time(t_ping_stats *stats)
{
	struct timeval	current_time;
	long		total_ms;

	if (!stats)
		return (0);
	gettimeofday(&current_time, NULL);
	total_ms = (current_time.tv_sec - stats->start_moment.tv_sec) * 1000;
	total_ms += (current_time.tv_usec - stats->start_moment.tv_usec) / 1000;
	return (total_ms);
}

static void	print_packet_summary(t_ping_stats *stats)
{
	double packet_loss = get_packet_loss_percent(stats);

	if (packet_loss == 0.0)
		printf("%d packets transmitted, %d received, 0%% packet loss",
			stats->packets_sent, stats->packets_got);
	else
		printf("%d packets transmitted, %d received, %.0f%% packet loss",
			stats->packets_sent, stats->packets_got, packet_loss);
}

static void	print_time_stats(t_ping_stats *stats)
{
	if (stats->packets_got > 0) {
		double avg_time = get_average_time(stats);
		double time_variation = get_time_variation(stats);
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			stats->fastest_time, avg_time, stats->slowest_time, time_variation);
	}
}

void	print_final_stats(t_ping_stats *stats, const char *target)
{
	if (!stats || !target)
		return ;

	long total_time = get_total_run_time(stats);
	printf("\n--- %s ping statistics ---\n", target);
	print_packet_summary(stats);
	printf(", time %ldms\n", total_time);
	print_time_stats(stats);
}