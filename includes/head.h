/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/11/23 13:57:01 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <fcntl.h>
# include <signal.h>
# include <time.h>

// Define the Packet Constants
// ping packet size
# define PING_PKT_S			64

// Automatic port number
# define PORT_NO			0

// Automatic port number
# define PING_SLEEP_RATE	1000000

// Gives the timeout delay for receiving packets
// in seconds
# define RECV_TIMEOUT		1



// ping packet structure
struct					ping_pkt
{
	struct icmphdr		hdr;
	char				msg[PING_PKT_S-sizeof(struct icmphdr)];
};

typedef struct			s_stats
{
	long double 		rtt_msec;
	long double 		total_msec;
	struct timeval		tv_out;
	struct timespec		time_start;
	struct timespec		time_end;
	struct timespec		tfs;
	struct timespec		tfe;
	int					msg_count;
	int					msg_received_count;
}						t_stats;

typedef struct			s_infos
{
	int					sockfd;
	struct sockaddr_in	ping_recv;
	struct sockaddr_in	ping_dest;
	int					ttl_val;
	char				*reverse_hostname;
	char				*dest;
	char				*user_input;
	struct ping_pkt		pckt;
	t_stats				stats;
}						t_infos;

# include "prototypes_ping.h"

#endif
