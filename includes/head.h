/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/12/09 20:17:28 by amartinod        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include "prototypes_ping.h"

#include <stdio.h> // printf family
#include <sys/types.h> // getaddrinfo
#include <sys/socket.h> //socket, setsockopt and getaddrinfo
#include <netdb.h> // getaddrinfo
#include <string.h> // memset pas autorisé mais chaud de laisser. #libft_free
#include <unistd.h>
#include <arpa/inet.h> //inet_ntop and inet_pton
#include <sys/time.h> // get time of the day and timeval struct
#include <signal.h> // fonction homonyme

# define FAILURE		-1
// Gives the timeout delay for receiving packets in seconds 
#define RECV_TIMEOUT 1 

typedef struct			s_infos
{
	struct addrinfo		*addr;
	struct timeval		time_value;
	// https://sites.uclouvain.be/SystInfo/usr/include/netinet/ip_icmp.h.html
	// header are great but message is missing, maybe check in ip.h ?
	struct icmphdr		icmp_header;
}						t_infos;

/*
** struct addrinfo {
**                int              ai_flags;
**                int              ai_family;
**                int              ai_socktype;
**                int              ai_protocol;
**                socklen_t        ai_addrlen;
**                struct sockaddr *ai_addr;
**                char            *ai_canonname;
**                struct addrinfo *ai_next;
**            };
** 
struct sockaddr_in {
**  	sa_family_t    sin_family; // famille d'adresses : AF_INET     
**     uint16_t       sin_port;    // port dans l'ordre d'octets réseau 
**     struct in_addr sin_addr;    // adresse Internet                 
** };
** 
** struct in_addr {
**     uint32_t       s_addr;      // Adresse dans l'ordre d'octets réseau
** };
**
** struct timeval {
**         time_t       tv_sec;   // seconds since Jan. 1, 1970
**         suseconds_t  tv_usec;  // and microseconds 
** };
*/

void				print_ip(struct addrinfo *p);
int	    			open_socket_and_set_options(void);
struct addrinfo		*dns_lookup(const char *url);

/*
**	This function format the ICMP datagram
*/
void				*get_icmp_hdr(void *ip_hdr, void *data, size_t len);

/*
**	This function send the icmp request
*/
// Does sendto fills all the different OSI protocol headers ?
void				*send_icmp_to_addr(void *ip_hdr, void *data, size_t len);

/*
**	This function listen for answers and deduce for each the time took, and other infos
*/
// How to know which answer is for which message ?
void				*listen_for_answers(void);


#endif
