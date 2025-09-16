/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:41:30 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/16 17:50:17 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_H
# define TYPE_H

# include <sys/time.h>
# include <stdint.h>

/* Argumentos del programa */
typedef struct s_args
{
	char	*flag;
	char	*dest;
	int		dest_allocated;  // 1 if dest was dynamically allocated, 0 if not
	int		verbose;
}	t_args;

/* Ping statistics */
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

#endif