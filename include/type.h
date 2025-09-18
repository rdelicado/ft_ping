/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:41:30 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/17 08:22:02 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_H
# define TYPE_H

# include <sys/time.h>
# include <stdint.h>

/* Argumentos del programa */
typedef struct s_args
{
	// opciones de flags
	int				verbose;
	int				help;
	int				count; // numero de paquetes para -c

	// destino
	char			*dest;
	int				dest_allocated; // 1 if dest was dynamically allocated

	// configuracion adicional para mas flags
	int				flood; // para -f
	int				packet_size; // para -s
	int				ttl;			//para --ttl
}	t_args;

/* Ping statistics */
typedef struct s_ping_stats
{
	int				packets_transmitted;
	int				packets_received;
	double			min_rtt;
	double			max_rtt;
	double			sum_rtt;
	double			sum_rtt_squared;
	struct timeval	start_time;
	struct timeval	end_time;
}	t_ping_stats;

#endif