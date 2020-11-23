/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/11/23 16:40:16 by amartinod        ###   ########.fr       */
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

typedef FAILURE		-1

void	print_ip(struct addrinfo *p);

#endif
