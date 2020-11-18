/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:57:33 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/11 15:00:31 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

u_int16_t		checksum_it(void *data, size_t size)
{
	uint16_t *addr = data;
	int count = size;
	register uint32_t sum = 0;
	uint16_t answer = 0;

	// Sum up 2-byte values until none or only one byte left.
	while (count > 1)
	{
		sum += *(addr++);
		count -= 2;
	}
	// Add left-over byte, if any.
	if (count > 0)
		sum += *(uint8_t *) addr;
	// Fold 32-bit sum into 16 bits; we lose information by doing this,
	// increasing the chances of a collision.
	// sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	// Checksum is one's compliment of sum.
	answer = ~sum;
	return (answer);
}

/*
**	This function format the ICMP datagram
*/
void				get_icmp_hdr(t_infos *ping, struct icmphdr *icmp_hdr)
{
	(void)ping;
	// Message Type (8 bits): echo request
	icmp_hdr->type = ICMP_ECHO;
	// Message Code (8 bits): echo request
	icmp_hdr->code = 0;
	icmp_hdr->checksum = 0;//Echo reply
	// Identifier (16 bits): usually pid of sending process - pick a number
	icmp_hdr->un.echo.id = htons(getpid());//Echo reply
	icmp_hdr->un.echo.sequence = htons(0);//Echo reply
	icmp_hdr->checksum = 0;
	icmp_hdr->checksum = checksum_it(icmp_hdr, sizeof(t_ping_pkt) - sizeof(struct ip));
}

void				get_ip_hdr(t_infos *ping, struct ip *ip_hdr)
{
	int		ip_flags[4];
	int		status;

	(void)ping;
	// Internet Protocol version (4 bits): IPv4
	ip_hdr->ip_v = 4;
	ip_hdr->ip_hl = sizeof(*ip_hdr) >> 2;
	// IPv4 header length (4 bits): Number of 32-bit words in header = 5
    ip_hdr->ip_hl = IP4_HDRLEN / sizeof (uint32_t);
	// Type of service (8 bits)
	ip_hdr->ip_tos = 0;
	// Total length of datagram (16 bits): IP header + ICMP header + ICMP data
	ip_hdr->ip_len = htons(sizeof(t_ping_pkt));
	ip_hdr->ip_len = htons(IP4_HDRLEN + ICMP_HDRLEN + PING_PKT_SIZE);
	// ID sequence number (16 bits): unused, since single datagram
	ip_hdr->ip_id = htons(0);

	// Flags, and Fragmentation offset (3, 13 bits): 0 since single datagram
	// Zero (1 bit)
	ip_flags[0] = 0;
	// Do not fragment flag (1 bit)
	ip_flags[1] = 0;
	// More fragments following flag (1 bit)
	ip_flags[2] = 0;
	// Fragmentation offset (13 bits)
	ip_flags[3] = 0;

	// ip_hdr->ip_off |= htons(IP_DF);
    ip_hdr->ip_off = htons ((ip_flags[0] << 15)
                        + (ip_flags[1] << 14)
                        + (ip_flags[2] << 13)
                        +  ip_flags[3]);

	// Time-to-Live (8 bits): default to maximum value
	ip_hdr->ip_ttl = 255;//TTL_VALUE;
	// Tells the Network layer at the destination host,,
	// to which Protocol this packet belongs to, i.e. the next level Protocol.
	// For example protocol number of ICMP is 1, TCP is 6 and UDP is 17.
    // Transport layer protocol (8 bits): 1 for ICMP
    ip_hdr->ip_p = IPPROTO_ICMP;
	// Source IPv4 address (32 bits)
	if ((status = inet_pton (AF_INET, ping->src_addr, &(ip_hdr->ip_src))) != 1)
	{
		fprintf (stderr, "inet_pton() failed.\nError message: %s", strerror (status));
		exit (EXIT_FAILURE);
	}
	// Destination IPv4 address (32 bits)
	if ((status = inet_pton (AF_INET, ping->dst_addr, &(ip_hdr->ip_dst))) != 1)
	{
		fprintf (stderr, "inet_pton() failed.\nError message: %s", strerror (status));
		exit (EXIT_FAILURE);
	}
	ip_hdr->ip_sum = 0;
	ip_hdr->ip_sum = checksum_it(ip_hdr, sizeof(*ip_hdr));
}

void		fill_ping_packet(t_infos *ping)
{
	memset(&ping->packet, 0, sizeof(t_ping_pkt));
	// ICMP data
    ping->packet.msg[0] = 'T';
    ping->packet.msg[1] = 'E';
    ping->packet.msg[2] = 'S';
    ping->packet.msg[3] = 'T';
	get_ip_hdr(ping, &ping->packet.ip_hdr);
	get_icmp_hdr(ping, &ping->packet.icmp_hdr);
}
