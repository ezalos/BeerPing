#include "head.h"

typedef struct			s_infos
{
	struct addrinfo		*addr;
	int					name1;
	int					name2;
}						t_infos;

/*
**	From a string containing an URL, get us the struct addrinfo
** FOR REVERSE DNS
** Historically, passing a host's own hostname to getaddrinfo() has been a popular
** technique for determining that host's IP address(es), but this is fragile,
** and doesn't work reliably in all cases. The appropriate way for software to
** discover the IP address(es) of the host it is running on is to use getifaddrs(3).
*/
struct addrinfo		*dns_lookup(const char *url)
{
	struct addrinfo   hints;
    struct addrinfo   *res;
    int               error;

    res = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    error = getaddrinfo(url, NULL, &hints, &res);
    if (error)
    {
        printf("error is : %s\n", gai_strerror(error));
        return NULL;
    }
    return res;
}


/*
 * man icmp
 * Outgoing packets automatically have an IP header prepended to them
 * (based on the destination address).
 * Incoming packets are received with the IP header and options intact.
 * ICMP sockets can be opened with the SOCK_DGRAM socket type without requiring
 * root privileges. Datagram oriented ICMP sockets offer a subset of the
 * functionality available to raw ICMP sockets.
 *
 * man setsockopt
 * To manipulate options at the socket level, level is specified as SOL_SOCKET.
 * To manipulate options at any other level the protocol number of the appropriate
 * protocol controlling the option is supplied. See man ip for the protocol number
 *
 * Option at IP level (man ip)
 * There are several IP-level setsockopt(2) /getsockopt(2) options.
 * IP_TTL may be used to set the time-to-live fields in the IP header for SOCK_DGRAM
 *
 * int ttl = 60; (max = 255)
 * setsockopt(s, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
 *
 * Option at socket level (man setsockopt)
 * SO_RCVTIMEO is an option to set a timeout value for input operations.
 * It accepts a struct timeval parameter with the number of seconds.
*/ 
// Gives the timeout delay for receiving packets 
// in seconds 
#define RECV_TIMEOUT 1 
int	    open_socket_and_set_options(void)
{
    int             sock_fd;
    int             ret;
    int             ttl;
    struct timeval  tv_input;

    ttl = 64;
    tv_input.tv_sec = RECV_TIMEOUT;
    tv_input.tv_usec = 0;
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock_fd < 0)
    {
        printf("The opening of the socket failed\n");
        return (sock_fd);
    }
    ret = setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    if (ret != FAILURE)
        ret = setsockopt(sock_fd, SOL_SOCKET, &tv_input, sizeof(tv_input));
    if (ret == FAILURE)
    {
        printf("Failed to set socket options\n");
        return (FAILURE); // should I close the socket ?
    }
    return (sock_fd);
}


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

int		main(int ac, char **av)
{
	t_infos		info;
    int         sock_fd;
	char 		ip_str[INET6_ADDRSTRLEN];

    if (ac > 1)
    {
        info.addr = dns_lookup(av[1]);
        inet_ntop(info.addr->ai_family, info.addr, ip_str, sizeof(ip_str));
        sock_fd = open_socket_and_set_options();
        printf("sock return : %d\n", sock_fd);
        print_ip(info.addr);
    }
    else
        printf("Please enter an url\n");
	return (0);
}
