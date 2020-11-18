#include "head.h"

typedef struct			s_infos
{
	struct addrinfo		*addr;
	int					name1;
	int					name2;
}						t_infos;

/*
**	From a string containing an URL, get us the struct addrinfo
*/
struct addrinfo		*get_addr_info_from_url(const char *url)
{
	struct addrinfo   hints;
    struct addrinfo   *res;
    int               error;

    res = NULL;
    memset(&hints, 0, sizeof(hints));
    error = getaddrinfo(url, NULL, &hints, &res);
    if (error)
    {
        printf("error is : %s\n", gai_strerror(error));
        return NULL;
    }
    return res;
}


/*
**	This function takes care of creating a ready to use socket for communication
**	with the struct addrinfo
**	setsockopt may be need to be called
*/
// Does socket takes care of internet communication ?
int					open_socket_for_communication_with_server(struct addrinfo *addr);


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
    (void)ac;
	(void)av;
	t_infos		info;

	info.addr = get_addr_info_from_url("www.google.com");
    print_ip(info.addr);
	return (0);
}
