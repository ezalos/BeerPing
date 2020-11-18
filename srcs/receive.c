/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 15:03:40 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/11 15:03:40 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "head.h"
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
	int					i;


	memset(&msg, 0, sizeof(msg));
	// memset((ping->buffer), 0, BUFFER_LEN);
	// memset((ping->buffer_ctrl), 0, BUFFER_CTRL_LEN);
	memset(&(ping->iov), 0, sizeof(struct iovec));
	msg.msg_name = ping->addr->ai_addr;
	msg.msg_namelen = ping->addr->ai_addrlen;
	msg.msg_iov = &ping->iov;
	// msg.msg_iov->iov_base = &ping->buffer;
	msg.msg_iov->iov_len = BUFFER_LEN;
	msg.msg_iovlen = 1;
	// msg.msg_control = ping->buffer_ctrl;
	msg.msg_controllen = BUFFER_CTRL_LEN;
	msg.msg_flags = 0;
	flags = MSG_WAITALL;//MSG_DONTWAIT
	// flags = MSG_DONTWAIT;//MSG_DONTWAIT
	retval = recvmsg(ping->sockid, &msg, flags);
	printf("RCVMSG: %ld\n", retval);
	if (retval == -1)
		perror("RECVMSG");
	else
	{
		i = -1;
		while (++i < (int)msg.msg_controllen)
			printf("%02hhx ", ((uint8_t*)msg.msg_control)[i]);
		printf("\n");
	}
}
