/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdelicad <rdelicad@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:41:30 by rdelicad          #+#    #+#             */
/*   Updated: 2025/09/13 15:53:46 by rdelicad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

typedef struct s_args
{
	char	*flag;
	char	*dest;
	int		dest_allocated;  // 1 si dest fue asignado dinámicamente, 0 si no
}	t_args;