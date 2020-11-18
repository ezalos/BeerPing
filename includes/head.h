/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/11/11 15:14:41 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H


# include <time.h>
#include <errno.h>            // errno, perror()
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <linux/icmp.h>
# include <sys/types.h>
#include <linux/if_ether.h>   // ETH_P_IP = 0x0800, ETH_P_IPV6 = 0x86DD
# include <unistd.h>
#include <net/if.h>           // struct ifreq
// #include <bits/ioctls.h>      // defines values for argument "request" of ioctl.
#include <sys/ioctl.h>        // macro ioctl is defined
#include <linux/if_packet.h>  // struct sockaddr_ll (see man 7 packet)
#include <net/ethernet.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

# define ERROR				-1
# define SUCCESS			0
# define FALSE				0
# define TRUE				1
// Define the Packet Constants
// ping packet size
# define PING_PKT_SIZE		4
# define BUFFER_LEN			sizeof(t_ping_pkt)
# define BUFFER_CTRL_LEN	420
#define IP4_HDRLEN 20         // IPv4 header length
#define ICMP_HDRLEN 8         // ICMP header length for echo request, excludes data
#define TTL_VALUE	64

// ping packet structure
typedef struct 				s_ping_pkt
{
	struct ethhdr			eth_hdr;
	struct ip				ip_hdr;
    struct icmphdr			icmp_hdr;
    char					msg[PING_PKT_SIZE];
}							t_ping_pkt;

typedef struct			s_infos
{
	struct addrinfo		*addr;
	struct sockaddr_in	sin;
	char				*dst_addr;
	char				*src_addr;
	char				dst_mac[6];
	char				src_mac[6];
	char				buffer_ctrl[BUFFER_CTRL_LEN];
	int					sockid;
	struct iovec		iov;
	struct sockaddr_ll	device;
	struct ifreq		ifr;
	t_ping_pkt			packet;
}						t_infos;



// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1


// struct						ip
// {
// #if BYTE_ORDER == LITTLE_ENDIAN
//     u_char  				ip_hl:4,        /* header length */
//         					ip_v:4;         /* version */
// #endif
// #if BYTE_ORDER == BIG_ENDIAN
//     u_char  				ip_v:4,         /* version */
//         					ip_hl:4;        /* header length */
// #endif
//     u_char  				ip_tos;         /* type of service */
//     short   				ip_len;         /* total length */
//     u_short 				ip_id;          /* identification */
//     short   				ip_off;         /* fragment offset field */
// #define IP_DF 0x4000         			/* dont fragment flag */
// #define IP_MF 0x2000           			/* more fragments flag */
//     u_char  				ip_ttl;         /* time to live */
//     u_char  				ip_p;           /* protocol */
//     u_short 				ip_sum;         /* checksum */
//     struct  in_addr 		ip_src,ip_dst;  /* source and dest address */
// };

// struct					icmphdr
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


# include "prototypes_ping.h"
#endif
