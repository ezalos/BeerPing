#include "head.h"

// make a ping request
//(int ping_sockfd, struct sockaddr_in *ping_addr,
//char *ping_dom, char *ping_ip, char *rev_host)

void		init_send(t_infos *ping)
{
	// struct ping_pkt pckt;
	ping->ttl_val = 64;
	ping->stats.rtt_msec = 0;
	ping->stats.total_msec = 0;
	ping->stats.tv_out.tv_sec = RECV_TIMEOUT;
	ping->stats.tv_out.tv_usec = 0;
	ping->stats.msg_count = 0;
	ping->stats.msg_received_count = 0;

	clock_gettime(CLOCK_MONOTONIC, &ping->stats.tfs);


	// set socket options at ip to TTL and value to 64,
	// change to what you want by setting ttl_val
	if (setsockopt(ping->sockfd, SOL_IP, IP_TTL,
			&ping->ttl_val, sizeof(ping->ttl_val)) != 0)
	{
		printf("\nSetting socket options to TTL failed!\n");
		return;
	}

	else
	{
		printf("\nSocket set to TTL..\n");
	}

	// setting timeout of recv setting
	setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO,
			(const char*)&ping->stats.tv_out, sizeof(ping->stats.tv_out));

}

void		fill_packet(t_infos *ping)
{
	int i;
	bzero(&ping->pckt, sizeof(ping->pckt));

	ping->pckt.hdr.type = ICMP_ECHO;
	ping->pckt.hdr.un.echo.id = getpid();

	i = 0;
	while (i < (int)sizeof(ping->pckt.msg) - 1)
	{
		ping->pckt.msg[i] = i+'0';
		i++;
	}
	ping->pckt.msg[i] = '\0';
	ping->pckt.hdr.un.echo.sequence = ping->stats.msg_count++;
	ping->pckt.hdr.checksum = checksum(&ping->pckt, sizeof(ping->pckt));
}

void		send_ping(t_infos *ping)
{
	int		retval;

	init_send(ping);
	while(*singletone_ping())
	{
		usleep(PING_SLEEP_RATE);
		fill_packet(ping);
		clock_gettime(CLOCK_MONOTONIC, &ping->stats.time_start);
		retval = sendto(ping->sockfd,
					&ping->pckt,
					sizeof(ping->pckt),
					0,
					(struct sockaddr*) &ping->ping_dest,
					sizeof(ping->ping_dest));
		if (retval <= 0)
			printf("\nPacket Sending Failed!\n");
		else
			receive_ping(ping);
	}
	print_results(ping);
}
