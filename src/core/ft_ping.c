/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:44 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 19:10:29 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	print_help(void)
{
	const char	*help[] = {
		"Usage\n"
		"  ft_ping [options] <destination>",
		"\n"
		"Options:",
		"  <destination>		DNS name or IP address",
		"  -v			verbose output",
		"  -?			print help and exit",
		"",
		"Example:",
		"  sudo ./ft_ping google.com",
		"  sudo ./ft_ping -v google.com",
		NULL
	};

	for (int i = 0; help[i]; i++)
		puts(help[i]);
}

void	cleanup_args(t_args *args)
{
	if (args && args->dest && args->dest_allocated) {
		free(args->dest);
		args->dest = NULL;
		args->dest_allocated = 0;
	}
}

void	convert_ip_binary(int sockfd, t_args *args, struct sockaddr_in *dest_addr)
{
	// Resolver el destino a IP binaria
	if (resolve_destination(args->dest, dest_addr) == 0) {
		printf("ft_ping: %s: Name or service not known\n", args->dest);
		close_socket(sockfd);
		cleanup_args(args);
		exit(2);
	}

	// mostrar ip resuelta
	printf("PING %s (%s): 56 data bytes\n", args->dest, inet_ntoa(dest_addr->sin_addr));
}

void	ping_loop(int sockfd, struct sockaddr_in *dest_addr)
{
    struct timeval	send_time;
    uint16_t		packet_id;
    uint16_t		sequence;
    int				seq_counter;

    seq_counter = 1;
    while (!g_stop)
    {
        // Marcar tiempo de envío
        gettimeofday(&send_time, NULL);
        
        // Enviar paquete ICMP
        if (icmp_request(sockfd, dest_addr, &packet_id, &sequence, seq_counter) == 0)
        {
            // Esperar y procesar respuesta
            if (icmp_receive(sockfd, packet_id, &send_time) < 0)
            {
                printf("Request timeout for icmp_seq=%d\n", seq_counter);
            }
        }
        else
        {
            printf("Error enviando paquete icmp_seq=%d\n", seq_counter);
        }
        
        // Verificar si se debe parar antes de incrementar y dormir
        if (g_stop) break;
        
        seq_counter++;
        sleep(1);
    }
}

int main(int ac, char **av)
{
    struct sockaddr_in	dest_addr;
    t_args				args;
    int					sockfd;

    setup_signal_handler();
    memset(&args, 0, sizeof(t_args));
    parse_arguments(ac, av, &args);
    sockfd = create_socket(0);
    if (sockfd < 0) {
        printf("ft_ping: Operacion no permitida (necesitas sudo)\n");
        cleanup_args(&args);
        return 1;
    }
    convert_ip_binary(sockfd, &args, &dest_addr);
    
    // Bucle principal de ping
    ping_loop(sockfd, &dest_addr);
    
    close_socket(sockfd);
    cleanup_args(&args);
    return 0;
}
