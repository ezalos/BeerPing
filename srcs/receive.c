/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 13:55:42 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/23 13:55:44 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		receive_time(t_infos *ping)
{
	clock_gettime(CLOCK_MONOTONIC, &ping->stats.time_end);

	double timeElapsed = ((double)(ping->stats.time_end.tv_nsec -
						ping->stats.time_start.tv_nsec))/1000000.0;
	ping->stats.rtt_msec = (ping->stats.time_end.tv_sec-
				ping->stats.time_start.tv_sec) * 1000.0
				+ timeElapsed;
}

void		receive_ping(t_infos *ping)
{
	int				retval;
	socklen_t		addr_len_int = sizeof(ping->ping_recv);

	retval = recvfrom(ping->sockfd,
			&ping->pckt,
			sizeof(ping->pckt),
			0,
			(struct sockaddr*)&ping->ping_recv,
			&addr_len_int);

	if (retval <= 0 && ping->stats.msg_count > 1)
		printf("\nPacket receive failed!\n");
	else
	{
		receive_time(ping);
		if(!(ping->pckt.hdr.type ==69 && ping->pckt.hdr.code==0))
		{
			printf("Error..Packet received with ICMP type %d code %d\n",
				ping->pckt.hdr.type, ping->pckt.hdr.code);
		}
		else
		{
			printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n",
				PING_PKT_S, ping->reverse_hostname, ping->user_input,
				ping->dest, ping->stats.msg_count,
				ping->ttl_val, ping->stats.rtt_msec);

			ping->stats.msg_received_count++;
		}
	}
}
