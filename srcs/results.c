/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   results.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 14:01:00 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/23 14:01:25 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		print_results(t_infos *ping)
{
	clock_gettime(CLOCK_MONOTONIC, &ping->stats.tfe);
	double timeElapsed = ((double)(ping->stats.tfe.tv_nsec -
						ping->stats.tfs.tv_nsec)) / 1000000.0;

	ping->stats.total_msec = (ping->stats.tfe.tv_sec - ping->stats.tfs.tv_sec)
			* 1000.0 + timeElapsed;

	printf("\n===%s ping statistics===\n", ping->dest);
	printf("\n%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n",
		ping->stats.msg_count, ping->stats.msg_received_count,
		((ping->stats.msg_count - ping->stats.msg_received_count)/ping->stats.msg_count) * 100.0,
		ping->stats.total_msec);
}
