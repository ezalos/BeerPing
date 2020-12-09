#include "head.h"

/*
** From a string containing an URL, get us the struct addrinfo
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
        fprintf(stderr, "error is : %s\n", gai_strerror(error));
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
        fprintf(stderr, "The opening of the socket failed\n");
        return (sock_fd);
    }
    ret = setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    if (ret != FAILURE)
        ret = setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv_input, sizeof(tv_input));
    if (ret == FAILURE)
    {
        fprintf(stderr, "Failed to set socket options\n");
        return (FAILURE); // should I close the socket ?
    }
    return (sock_fd);
}
