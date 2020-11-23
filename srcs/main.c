/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 14:02:13 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/23 14:02:27 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int		main(int ac, char *av[])
{
	t_infos				ping;

	if (ac != 2)
	{
		printf("Usage: %s <address>\n", av[0]);
		return 0;
	}
	bzero(&ping, sizeof(ping));
	ping.user_input = av[1];
	ping.dest = dns_lookup(ping.user_input, &ping.ping_dest);
	if(ping.dest == NULL)
	{
		printf("\nDNS lookup failed! Could not resolve hostname!\n");
		return 0;
	}

	ping.reverse_hostname = reverse_dns_lookup(ping.dest);
	printf("\nTrying to connect to '%s' IP: %s\n", av[1], ping.dest);
	printf("\nReverse Lookup domain: %s", ping.reverse_hostname);

	//socket()
	ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(ping.sockfd < 0)
	{
		printf("\nSocket file descriptor not received!!\n");
		return 0;
	}
	else
		printf("\nSocket file descriptor %d received\n", ping.sockfd);

	signal(SIGINT, intHandler);
	send_ping(&ping);

	return 0;
}
