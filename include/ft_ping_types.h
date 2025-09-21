/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping_types.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 00:00:00 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/21 00:00:00 by rdelicado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_TYPES_H
#define FT_PING_TYPES_H

#include <sys/time.h>
#include <stdint.h>
#include <netinet/in.h>
#include <signal.h>  /* Para sig_atomic_t */

/* ===== ESTRUCTURAS Y TIPOS ===== */

/* Argumentos del programa */
typedef struct s_args
{
	// opciones de flags
	int				mode_verbose;
	int				show_help;
	int				packet_count; // numero de paquetes para -c

	// destino
	char			*target;
	int				target_was_created; // 1 if target was dynamically allocated

	// configuracion adicional para mas flags
	int				flood_mode; // para -f
	int				packet_bytes; // para -s
	int				time_to_live;			//para --ttl
}	t_args;

/* Ping statistics */
typedef struct s_ping_stats
{
	int				packets_sent;
	int				packets_got;
	double			fastest_time;
	double			slowest_time;
	double			total_time;
	double			total_time_squared;
	struct timeval	start_moment;
	struct timeval	end_moment;
}	t_ping_stats;

/* Ping context - agrupa datos relacionados del ping */
typedef struct s_ping_context
{
	int				socket_fd;
	struct sockaddr_in	*target_addr;
	uint16_t		packet_id;
	uint16_t		packet_number;
	int				next_number;
	t_ping_stats	*stats;
}	t_ping_context;

/* ===== VARIABLES GLOBALES ===== */
extern volatile sig_atomic_t g_stop;

#endif 