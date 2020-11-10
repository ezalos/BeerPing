/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 12:23:05 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/10 13:59:52 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"


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
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
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

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1
/*
**	This function takes care of creating a ready to use socket for communication with the struct addrinfo
**	setsockopt may be need to be called
*/
// Does socket takes care of internet communication ?
int					open_socket_for_communication_with_server(t_infos *ping)
{
	int		ttl_val=64;
	// int		msg_count=0;
	// int		i;
	// int		addr_len;
	// int		flag=1;
    // int		msg_received_count=0;


    struct timeval tv_out;

    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;



	if (-1 == (ping->sockid = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)))
	{
		fprintf(stderr, "socket: couldn't be created\n");
		perror("SOCKET");
		return (ERROR);
	}
    // set socket options at ip to TTL and value to 64,
    // change to what you want by setting ttl_val
    if (setsockopt(ping->sockid, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0)
    {
        printf("\nSetting socket options to TTL failed!\n");
		perror("SETSOCKOPT");
		return (ERROR);
    }
    else
    {
        printf("\nSocket set to TTL..\n");
    }

    // setting timeout of recv setting
    setsockopt(ping->sockid, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	return (0);
}


/*
**	This function format the ICMP datagram
*/
void				get_icmp_hdr(t_infos *ping, t_ping_pkt *data)
{
	(void)ping;
	memset(data, 0, sizeof(*data));
	data->hdr.type = 0;//Echo reply
	data->hdr.code = 0;//Echo reply
	data->hdr.checksum = 0;//Echo reply
	data->hdr.un.echo.id = 0;//Echo reply
	data->hdr.un.echo.sequence = 0;//Echo reply
}

/*
**	This function send the icmp request
*/
// Does sendto fills all the different OSI protocol headers ?
void				*send_icmp_to_addr(t_infos *ping)
{
	ssize_t				retval;
	t_ping_pkt			data;

	get_icmp_hdr(ping, &data);
	retval = sendto(ping->sockid,
				&data,
				sizeof(data),
				0,
				ping->addr->ai_addr,
				ping->addr->ai_addrlen);
	if (retval == -1)
	{
		perror("SENDTO");
		return (NULL);
	}
	return (ping);
}

/*
**	This function listen for answers and deduce for each the time took, and other infos
*/
// How to know which answer is for which message ?

// struct 				iovec                   /* Scatter/gather array items */
// {
//     void				*iov_base;              /* Starting address */
//     size_t			iov_len;           		/* Number of bytes to transfer */
// };
//
// struct				msghdr
// {
//     void         	*msg_name;       		/* optional address */
//     socklen_t     	msg_namelen;    		/* size of address */
//     struct iovec 	*msg_iov;        		/* scatter/gather array */
//     size_t        	msg_iovlen;     		/* # elements in msg_iov */
//     void         	*msg_control;    		/* ancillary data, see below */
//     size_t        	msg_controllen; 		/* ancillary data buffer len */
//     int           	msg_flags;      		/* flags on received message */
// };

void				listen_for_answers(t_infos *ping)
{
	ssize_t				retval;
	int					flags;
	struct msghdr		msg;

	flags = 0;
	memset(&msg, 0, sizeof(msg));
	msg.msg_name = ping->addr->ai_addr;
	msg.msg_namelen = ping->addr->ai_addrlen;
	retval = recvmsg(ping->sockid, &msg, flags);
	if (retval == -1)
		perror("RECVMSG");
	printf("RCVMSG: %ld\n", retval);
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
	open_socket_for_communication_with_server(&ping);
	send_icmp_to_addr(&ping);
	listen_for_answers(&ping);
	return (0);
}
