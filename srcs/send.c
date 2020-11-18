/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:59:18 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/11 14:59:58 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

/*
**	This function send the icmp request
*/
// Does sendto fills all the different OSI protocol headers ?
void				*send_packet(t_infos *ping)
{
	ssize_t				retval;

	fill_ping_packet(ping);
	prepare_socket(ping);
	// Send packet.
	retval = sendto(ping->sockid,
		&ping->packet,
		sizeof(t_ping_pkt),
		0,
		(struct sockaddr *) &ping->sin,
		sizeof(struct sockaddr));
	printf("SENDTO: %ld\n", retval);
	if (retval == -1)
	{
		perror("SENDTO");
		return (NULL);
	}
	return (ping);
}
