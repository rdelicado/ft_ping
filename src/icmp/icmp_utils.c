/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:42:42 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/15 17:44:44 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// Función para calcular el checksum ICMP (suma de 16 bits complementada)
uint16_t icmp_checksum(const void *buf, int len)
{
    uint32_t sum = 0;
    const uint16_t *ptr = buf;

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1)
        sum += *(const uint8_t *)ptr;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}