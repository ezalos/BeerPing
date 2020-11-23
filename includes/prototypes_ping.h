#ifndef PROTOTYPES_PING_H
# define PROTOTYPES_PING_H

uint16_t	checksum(void *b, int len);
char		*dns_lookup(char *user_input, struct sockaddr_in *ping_dest);
void		fill_packet(t_infos *ping);
void		init_send(t_infos *ping);
void		intHandler(int dummy);
int			main(int ac, char *av[]);
void		print_results(t_infos *ping);
void		receive_ping(t_infos *ping);
char		*reverse_dns_lookup(char *ip_addr);
void		send_ping(t_infos *ping);
int			*singletone_ping(void);

#endif