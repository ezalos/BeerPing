/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/11/10 13:29:43 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include "prototypes_ping.h"

# include <time.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <linux/icmp.h>

# define ERROR		-1
# define SUCCESS	0
# define FALSE		0
# define TRUE		1

typedef struct			s_infos
{
	struct addrinfo		*addr;
	int					sockid;
	int					name2;
}						t_infos;


// Define the Packet Constants
// ping packet size
#define PING_PKT_S 64

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1


// struct						icmphdr
// {
// 		u_int8_t			type;		/* message type */
// 		u_int8_t			code;		/* type sub-code */
// 		u_int16_t			checksum;
// 		union
// 		{
// 			struct
// 			{
// 				u_int16_t	id;
// 				u_int16_t	sequence;
// 			}				echo;		/* echo datagram */
// 			u_int32_t		gateway;	/* gateway address */
// 			struct
// 			{
// 				u_int16_t	__unused;
// 				u_int16_t	mtu;
// 			}				frag;		/* path mtu discovery */
// 		}					un;
// };


// ping packet structure
typedef struct 				s_ping_pkt
{
    struct icmphdr			hdr;
    char					msg[PING_PKT_S - sizeof(struct icmphdr)];
}							t_ping_pkt;

#endif
