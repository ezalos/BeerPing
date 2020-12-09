#include "head.h"

// while pingloop == 1, packet will be sent
int pingloop=1;

// on CTRL-C, set pingloop to 0 to terminate the process
void                terminate_ping_loop(int _)
{
    (void)_;
    // how can I clean my struct (addrifo) ?
    pingloop=0;
}

void                send_ping(int sock_fd, t_infos *info)
{
    int8_t      ret;

    (void)sock_fd;
    ret = gettimeofday(&info->time_value, NULL);
    if (ret == FAILURE)
    {
        fprintf(stderr, "error when getting time of the day, sorry bro\n");
        return ;
    }
    fprintf(stderr, "sec : %ld\nmicro sec : %d\n", info->time_value.tv_sec, info->time_value.tv_usec);
    while(pingloop == 1)
    {
        memset(&info.icmp_header, sizeof(info.icmp_header));
    }
}

int		main(int ac, char **av)
{
	t_infos		info;
    int         sock_fd;
	char 		ip_str[INET6_ADDRSTRLEN];

    if (ac > 1)
    {
        memset(&info, 0, sizeof(info));
        info.addr = dns_lookup(av[1]);
        inet_ntop(info.addr->ai_family, info.addr, ip_str, sizeof(ip_str));
        sock_fd = open_socket_and_set_options();
        fprintf(stderr, "sock return : %d\n", sock_fd);
        print_ip(info.addr);
        signal(SIGINT, terminate_ping_loop);
        send_ping(sock_fd, &info);
    }
    else
        fprintf(stderr, "Please enter an url\n");
	return (0);
}
