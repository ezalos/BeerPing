/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 12:23:05 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/18 19:02:27 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	*get_ipntop(struct addrinfo *p)
{
	int					status;
	struct sockaddr_in	*sa_in;
	char				*ip_share;

	if ((ip_share = malloc(INET_ADDRSTRLEN)) == NULL)// TODO: utiliser cst mem
	{
		fprintf(stderr, "Error malloc\n");
		return (NULL);
	}
	sa_in = (struct sockaddr_in *)p->ai_addr;
	if (inet_ntop(p->ai_family, &(sa_in->sin_addr), ip_share, INET_ADDRSTRLEN) == NULL)
	{
      status = errno;
      fprintf (stderr, "inet_ntop() failed.\nError message: %s", strerror (status));
      exit (EXIT_FAILURE);
    }
	return ip_share;
}

void	*get_ipv4(struct addrinfo *p)
{
	while (p != NULL)
	{
		if (p->ai_family == AF_INET)
			return p;
		p = p->ai_next;
	}
	return NULL;
}

/*
**	From a string containing an URL, get us the struct addrinfo
*/
struct addrinfo		*get_addr_info_from_url(const char *url)
{
	struct addrinfo		hints;
	int					status;
	struct addrinfo		*res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_RAW; // Une seule famille de socket
	hints.ai_protocol = IPPROTO_ICMP;
	// hints.ai_flags = hints.ai_flags | AI_CANONNAME;
	// getaddrinfo("URL", "PORT", hints, flags)
	if ((status = getaddrinfo(url, NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (NULL);
	}
	printf("IP addresses for %s:\n\n", url);
	print_ip(res);
	// freeaddrinfo(res);
	return get_ipv4(res);
}

/*
**	This function takes care of creating a ready to use socket for communication with the struct addrinfo
**	setsockopt may be need to be called
*/
// Does socket takes care of internet communication ?
int					open_socket_for_communication_with_server(t_infos *ping)
{
	// int					ttl_val=TTL_VALUE;
	struct addrinfo		hints;
	struct addrinfo		*res;
	int					status;
	// int		msg_count=0;
	// int		i;
	// int		addr_len;
	// int		flag=1;
    // int		msg_received_count=0;


	memset(&(hints), 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_RAW; // Une seule famille de socket
	hints.ai_protocol = IPPROTO_ICMP;
	if ((status = getaddrinfo(ping->dst_addr, NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (ERROR);
	}

    struct timeval tv_out;

    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;



	// if (-1 == (ping->sockid = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)))
	if (-1 == (ping->sockid = socket(res->ai_family, res->ai_socktype, res->ai_protocol)))
	{
		fprintf(stderr, "socket: couldn't be created\n");
		perror("SOCKET");
		return (ERROR);
	}
    // // set socket options at ip to TTL and value to 64,
    // // change to what you want by setting ttl_val
    // if ((status = setsockopt(ping->sockid, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val))) != 0)
    // {
    //     printf("\nSetting socket options to TTL failed!\n");
	// 	perror("SETSOCKOPT");
	// 	return (ERROR);
    // }
    // else
    // {
    //     printf("\nSocket set to TTL..\n");
    // }

    // setting timeout of recv setting
    if ((status = setsockopt(ping->sockid, IPPROTO_IP, IP_HDRINCL, (const char*)&tv_out, sizeof(tv_out))) < 0)
    {
        printf("\nSetting socket options to TTL failed!\n");
		perror("SETSOCKOPT");
		return (ERROR);
    }
    else
    {
        printf("\nSocket set to TTL..\n");
    }
	return (0);
}



void	prepare_socket(t_infos *ping)
{
	const int on = 1;
	// The kernel is going to prepare layer 2 information (ethernet frame header) for us.
	// For that, we need to specify a destination for the kernel in order for it
	// to decide where to send the raw datagram. We fill in a struct in_addr with
	// the desired destination IP address, and pass this structure to the sendto() function.
	memset (&ping->sin, 0, sizeof (struct sockaddr_in));
	ping->sin.sin_family = AF_INET;
	ping->sin.sin_addr.s_addr = ping->packet.ip_hdr.ip_dst.s_addr;

	// Submit request for a raw socket descriptor.
	if ((ping->sockid = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
	perror ("socket() failed ");
	exit (EXIT_FAILURE);
	}

	// Set flag so socket expects us to provide IPv4 header.
	if (setsockopt (ping->sockid, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0)
	{
		perror ("setsockopt() failed to set IP_HDRINCL ");
		exit (EXIT_FAILURE);
	}

	// Bind socket to interface index.
	if (setsockopt (ping->sockid, SOL_SOCKET, SO_BINDTODEVICE, &ping->ifr, sizeof (ping->ifr)) < 0)
	{
		perror ("setsockopt() failed to bind to interface ");
		exit (EXIT_FAILURE);
	}
}



int		main(int ac, char **av)
{
	t_infos		ping;

	if (ac != 2)
	{
		printf("usage: %s www.url.com\n", av[0]);
		return (0);
	}
	memset(&ping, 0, sizeof(t_infos));
	ping.addr = get_addr_info_from_url(av[1]);
	ping.dst_addr = get_ipntop(ping.addr);
	ping.src_addr = "0.0.0.0";// TODO check if src_addr is good
	check_interface(&ping);
	// open_socket_for_communication_with_server(&ping);
	send_packet(&ping);
	listen_for_answers(&ping);
	return (0);
}
