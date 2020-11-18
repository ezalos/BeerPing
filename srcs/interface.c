/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 14:58:43 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/11 15:09:42 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		check_interface(t_infos *ping)
{
	char				interface[7];
	int				i;

	(void)ping;
	// Interface to send packet through.
	strcpy (interface, "wlp2s0");

	// Submit request for a socket descriptor to look up interface.
	if ((ping->sockid = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0)
	{
		perror ("socket() failed to get socket descriptor for using ioctl() ");
		exit (EXIT_FAILURE);
	}

	// Use ioctl() to look up interface index which we will use to
	// bind socket descriptor sd to specified interface with setsockopt() since
	// none of the other arguments of sendto() specify which interface to use.
	memset (&ping->ifr, 0, sizeof (ping->ifr));
	snprintf (ping->ifr.ifr_name, sizeof (ping->ifr.ifr_name), "%s", interface);
	if (ioctl (ping->sockid, SIOCGIFHWADDR, &ping->ifr) < 0)
	{
		perror ("ioctl() failed to find interface ");
		return (ERROR);
	}
	printf ("Index for interface %s is %i\n", interface, ping->ifr.ifr_ifindex);
	// Copy source MAC address.
    memcpy (ping->src_mac, ping->ifr.ifr_hwaddr.sa_data, 6);

    // Report source MAC address to stdout.
    printf ("MAC address for interface %s is ", interface);
	i = -1;
    while (++i < 5)
		printf ("%02hhx:", ping->src_mac[i]);
    printf ("%02hhx\n", ping->src_mac[5]);
	// Find interface index from interface name and store index in
	// struct sockaddr_ll device, which will be used as an argument of sendto().
	memset (&ping->device, 0, sizeof (ping->device));
	if ((ping->device.sll_ifindex = if_nametoindex (interface)) == 0)
	{
		perror ("if_nametoindex() failed to obtain interface index ");
		exit (EXIT_FAILURE);
	}
	printf ("Index for interface %s is %i\n", interface, ping->device.sll_ifindex);

	// Set destination MAC address: you need to fill these out
	// char dest[6] = 0xE49E1202E65D;

	ping->dst_mac[0] = 0xE4;
	ping->dst_mac[1] = 0x9E;
	ping->dst_mac[2] = 0x12;
	ping->dst_mac[3] = 0x02;
	ping->dst_mac[4] = 0xE6;
	ping->dst_mac[5] = 0x5D;
	return (SUCCESS);
}
