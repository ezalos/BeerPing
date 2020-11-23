/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 13:45:52 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/23 14:02:06 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int		*singletone_ping(void)
{
	static int	pingloop = 1;

	return &pingloop;
}

void	intHandler(int dummy)
{
	(void)dummy;
	*singletone_ping()=0;
}
