#include "head.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

// int 	socket(int domain, int type, int protocol);//domain is PF_NDRV for MAcOs and AF_PACKET for linux
// 
// int 	setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
// 
// ssize_t sendto(int socket, const void *buffer, size_t length, int flags,
//     const struct sockaddr *dest_addr, socklen_t dest_len);
// 
// ssize_t recvfrom(int socket, void *restrict buffer, size_t length, int flags,
//     struct sockaddr *restrict address, socklen_t *restrict address_len);
// 
// //socket address structure to host and service name
// int 	getaddrinfo(const char *hostname, const char *servname,
//     const struct addrinfo *hints, struct addrinfo **res);
// 
// struct addrinfo {
//   int ai_flags;           /* input flags */
//   int ai_family;          /* protocol family for socket */
//   int ai_socktype;        /* socket type */
//   int ai_protocol;        /* protocol for socket */
//   socklen_t ai_addrlen;   /* length of socket-address */
//   struct sockaddr *ai_addr; /* socket-address for socket */
//   char *ai_canonname;     /* canonical name for service location */
//   struct addrinfo *ai_next; /* pointer to next in list */
// };
// 
// //Internet address manipulation routines
// const char * inet_ntop(int af, const void * restrict src, char * restrict dst, socklen_t size);
// 
// int inet_pton(int af, const char * restrict src, void * restrict dst);

int		open_socket(void)
{
    struct addrinfo   hints;
    struct addrinfo   *res;
    int               error;
//    int		          raw_socket;

    memset(&hints, 0, sizeof(hints));
   // hints.ai_family = PF_UNSPEC;
   // hints.ai_socktype = SOCK_RAW;

    error = getaddrinfo("www.google.com", NULL, &hints, &res);
    if (error)
    {
        printf("error is : %s\n", gai_strerror(error));
        return -1;
    }
    printf("ai_flags: %d\nai_family: %d\nai_socktype: %d\nai_protocol: %d\n", res->ai_flags, res->ai_family, res->ai_socktype, res->ai_protocol);
    //raw_socket = socket(PF_NDRV, SOCK_RAW, )
    return 0;
}

int		main(int ac, char **av)
{
    (void)av;
    (void)ac;
    printf("PID is : %d\nUID is : %d\n", getpid(), getuid());
    open_socket();
    return (0);
}
