/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 13:55:50 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/23 13:56:08 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

char		*dns_lookup(char *user_input, struct sockaddr_in *ping_dest)
{
	struct hostent			*host_entity;
	char					*ip=(char*)malloc(NI_MAXHOST*sizeof(char));

	printf("\nResolving DNS..\n");

	if ((host_entity = gethostbyname(user_input)) == NULL)
		return (NULL);

	//filling up address structure
	strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));

	ping_dest->sin_family = host_entity->h_addrtype;
	ping_dest->sin_port = htons (PORT_NO);
	ping_dest->sin_addr.s_addr = *(long*)host_entity->h_addr;

	return (ip);

}

char		*reverse_dns_lookup(char *ip_addr)
{
	struct sockaddr_in		temp_addr;
	socklen_t				len;
	char					buf[NI_MAXHOST];
	char					*ret_buf;

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
	len = sizeof(struct sockaddr_in);

	if (getnameinfo((struct sockaddr *) &temp_addr, len, buf,
					sizeof(buf), NULL, 0, NI_NAMEREQD))
	{
		printf("Could not resolve reverse lookup of hostname\n");
		return NULL;
	}
	ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) );
	strcpy(ret_buf, buf);
	return ret_buf;
}
