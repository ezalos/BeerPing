#include "head.h"



//
// // get socket:
// // http://manpagesfr.free.fr/man/man2/socket.2.html
//
// // ◦ getpid.
// // ◦ getuid.
//
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// struct addrinfo {
//                int              ai_flags;
//                int              ai_family;
//                int              ai_socktype;
//                int              ai_protocol;
//                socklen_t        ai_addrlen;
//                struct sockaddr *ai_addr;
//                char            *ai_canonname;
//                struct addrinfo *ai_next;
//            };

// struct sockaddr {
//    sa_family_t sa_family;
//    char        sa_data[14];
// }

// struct sockaddr_in {
//     sa_family_t    sin_family; /* address family: AF_INET */
//     in_port_t      sin_port;   /* port in network byte order */
//     struct in_addr sin_addr;   /* internet address */
// };

// /* Internet address. */
// struct in_addr {
//     uint32_t       s_addr;     /* address in network byte order */
// };

// int		getaddrinfo(const char *node, const char *service,
//                       const struct addrinfo *hints,
//                       struct addrinfo **res);
//
// // ◦ getaddrinfo.
// // ◦ gettimeofday.
// #include <arpa/inet.h>
//
// // src=struct in_addr	-> dest="123.56.7.89"
// const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
// // src="123.56.7.89"	-> dest=struct in_addr
// int inet_pton(int af, const char *src, void *dst);
//
// // ◦ exit.
// // ◦ signal.
// // ◦ alarm.
// // ◦ setsockopt.
// // ◦ recvmsg.
// // ◦ sendto.
// // ◦ socket.

// struct hostent {
//    char  *h_name;            /* Nom officiel de l'hôte. */
//    char **h_aliases;         /* Liste d'alias. */
//    int    h_addrtype;        /* Type d'adresse de l'hôte. */
//    int    h_length;          /* Longueur de l'adresse. */
//    char **h_addr_list;       /* Liste d'adresses. */
// }

#include <netdb.h>
#include <stdio.h>
void	reverse_dns(const char *url)
{
	struct hostent	*data;
	int				i;

	data = gethostbyname(url);
	if (data)
	{
		printf("h_name:         %s\n", data->h_name);
		i = -1;
		while (data->h_aliases[++i])
			printf("   h_aliases:   %s\n", data->h_aliases[i]);
		printf("h_addrtype:     %d -> %s\n", data->h_addrtype, data->h_addrtype == AF_INET ? "IPv4" : "IPv6");
		printf("h_length:       %d\n", data->h_length);
		i = -1;
		while (data->h_addr_list[++i])
			printf("   h_addr_list: %s\n", data->h_addr_list[i]);
	}

}


int		main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	reverse_dns(av[1]);
	return (0);
}
