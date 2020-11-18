#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

void	print_ip(struct addrinfo *p)
{
	void				*addr;
	char 				ipver;
	char 				ipstr[INET6_ADDRSTRLEN];

	while (p != NULL)
	{
		// Identification de l'adresse courante
		if (p->ai_family == AF_INET)
		{ // IPv4
			struct sockaddr_in		*ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = '4';
		}
		else
		{ // IPv6
			struct sockaddr_in6		*ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = '6';
		}
		// Conversion de l'adresse IP en une chaîne de caractères
		if (p->ai_family == AF_INET || p->ai_family == AF_INET6)
		{
			const char *tmp;
			tmp = inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
			if (tmp == ipstr)
				printf(" IPv%c: %s\n", ipver, ipstr);
		}
		// Adresse suivante
		p = p->ai_next;
	}
}
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

// struct sockaddr_in {
// 	sa_family_t    sin_family; /* famille d'adresses : AF_INET     */
//     uint16_t       sin_port;   /* port dans l'ordre d'octets réseau */
//     struct in_addr sin_addr;   /* adresse Internet                  */
// };

/* Adresse Internet */
// struct in_addr {
//     uint32_t       s_addr;     /* Adresse dans l'ordre d'octets réseau */
// };

int		fetch_ip_of_url(const char *url)
{
	struct addrinfo		hints;
	int					status;
	struct addrinfo		*res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
	hints.ai_socktype = SOCK_STREAM; // Une seule famille de socket
	// getaddrinfo("URL", "PORT", hints, flags)
	if ((status = getaddrinfo(url, NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}
	printf("IP addresses for %s:\n\n", url);
	print_ip(res);
	freeaddrinfo(res);
	return 0;
}
