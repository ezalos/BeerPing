#ifndef PROTOTYPES_PING_H
# define PROTOTYPES_PING_H

int					fetch_ip_of_url(const char *url);
struct addrinfo		*get_addr_info_from_url(const char *url);
int					main(int ac, char **av);
void				print_ip(struct addrinfo *p);

#endif