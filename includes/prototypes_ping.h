#ifndef PROTOTYPES_PING_H
# define PROTOTYPES_PING_H

int8_t				check_interface(t_infos *ping);
u_int16_t			checksum_it(void *data, size_t size);
int					fetch_ip_of_url(const char *url);
void				fill_ping_packet(t_infos *ping);
struct addrinfo		*get_addr_info_from_url(const char *url);
void				get_icmp_hdr(t_infos *ping, struct icmphdr *icmp_hdr);
void				get_ip_hdr(t_infos *ping, struct ip *ip_hdr);
void				*get_ipntop(struct addrinfo *p);
void				*get_ipv4(struct addrinfo *p);
void				listen_for_answers(t_infos *ping);
int					main(int ac, char **av);
int					open_socket_for_communication_with_server(t_infos *ping);
void				prepare_socket(t_infos *ping);
void				print_ip(struct addrinfo *p);
void				*send_packet(t_infos *ping);

#endif
