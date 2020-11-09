
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
// #include <netdb.h>
#include <arpa/inet.h>
// #include <linux/if_packet.h>
#include <net/ethernet.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

# define BUFFER_LEN		65536

void		extract_data(unsigned char *buffer, int buflen)
{
	struct iphdr 		*ip;
	unsigned short		iphdrlen;
	int					i;
	unsigned char 		*data;
	int 				remaining_data;

	ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
	iphdrlen = ip->ihl * 4;
	data = (buffer + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
	remaining_data = buflen - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
	for (i = 0; i < remaining_data; i++)
	{
		if(i!=0 && i%16==0)
			printf("\n");
		printf(" %.2X ",data[i]);
	}
}

// /* include/linux/tcp.h */
//
// struct tcphdr {
//     __u16   source;
//     __u16   dest;
//     __u32   seq;
//     __u32   ack_seq;
// #if defined(__LITTLE_ENDIAN_BITFIELD)
//     __u16   res1:4,
//         doff:4,
//         fin:1,
//         syn:1,
//         rst:1,
//         psh:1,
//         ack:1,
//         urg:1,
//         ece:1,
//         cwr:1;
// #elif defined(__BIG_ENDIAN_BITFIELD)
//     __u16   doff:4,
//         res1:4,
//         cwr:1,
//         ece:1,
//         urg:1,
//         ack:1,
//         psh:1,
//         rst:1,
//         syn:1,
//         fin:1;
// #else
// #error  "Adjust your <asm/byteorder.h> defines"
// #endif
//     __u16   window;
//     __u16   check;
//     __u16   urg_ptr;
// };


// struct udphdr {
// 	__be16	source;
// 	__be16	dest;
// 	__be16	len;
// 	__sum16	check;
// };

void		extract_udp_hdr(unsigned char *buffer)
{
	unsigned short		iphdrlen;
	struct iphdr 		*ip = (struct iphdr *)( buffer + sizeof(struct ethhdr) );

	/* getting actual size of IP header*/
	iphdrlen = ip->ihl*4;
	/* getting pointer to udp header*/
	struct udphdr *udp=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	printf("\nUDP Header\n");
	printf("\t|-Source Port : %d\n" , ntohs(udp->source));
	printf("\t|-Destination Port : %d\n" , ntohs(udp->dest));
	printf("\t|-UDP Length : %d\n" , ntohs(udp->len));
	printf("\t|-UDP Checksum : %d\n" , ntohs(udp->check));
}

// struct iphdr {
// #if defined(__LITTLE_ENDIAN_BITFIELD)
// 	__u8	ihl:4,
// 		version:4;
// #elif defined (__BIG_ENDIAN_BITFIELD)
// 	__u8	version:4,
//   		ihl:4;
// #else
// #error	"Please fix <asm/byteorder.h>"
// #endif
// 	__u8	tos;
// 	__u16	tot_len;
// 	__u16	id;
// 	__u16	frag_off;
// 	__u8	ttl;
// 	__u8	protocol;
// 	__u16	check;
// 	__u32	saddr;
// 	__u32	daddr;
// 	/*The options start here. */
// };
void		extract_ip_hdr(unsigned char *buffer)
{
	struct iphdr		*ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

	printf("\nIP Header\n");
	printf("\t|-Version : %d\n",(unsigned int)ip->version);
	printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
	printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
	printf("\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
	printf("\t|-Identification : %d\n",ntohs(ip->id));
	printf("\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
	printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);
	printf("\t|-Header Checksum : %d\n",ntohs(ip->check));

	// memset(&source, 0, sizeof(source));
	// source.sin_addr.s_addr = ip->saddr;
	// memset(&dest, 0, sizeof(dest));
	// dest.sin_addr.s_addr = ip->daddr;
	// printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
	// printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));
}

// struct ethhdr {
// 	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
// 	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
// 	__be16			h_proto;		/* packet type ID field	*/
// } __attribute__((packed));
void		extract_eth_hdr(unsigned char *buffer)
{
	struct ethhdr *eth = (struct ethhdr *)(buffer);

	printf("\nEthernet Header\n");
	printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
		eth->h_source[0],
		eth->h_source[1],
		eth->h_source[2],
		eth->h_source[3],
		eth->h_source[4],
		eth->h_source[5]);
	printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
		eth->h_dest[0],
		eth->h_dest[1],
		eth->h_dest[2],
		eth->h_dest[3],
		eth->h_dest[4],
		eth->h_dest[5]);
	printf("\t|-Protocol : %d\n",eth->h_proto);
}

int			reception_network_packet(int sock_r)
{
	unsigned char		*buffer;
	struct sockaddr		saddr;
	int					saddr_len = sizeof (saddr);
	int					buflen;

	buffer = malloc(BUFFER_LEN); //to receive data
	memset(buffer, 0, BUFFER_LEN);
	//Receive a network packet and copy in to buffer
	buflen = recvfrom(sock_r,
				buffer,
				BUFFER_LEN,
				0,
				&saddr,
				(socklen_t *)&saddr_len);
	if(buflen < 0)
	{
		printf("error in reading recvfrom function\n");
		return -1;
	}
	extract_eth_hdr(buffer);
	extract_ip_hdr(buffer);
	extract_udp_hdr(buffer);
	extract_data(buffer, buflen);
	printf("\n\n");
	return 0;
}

int			open_raw_socket(void)
{
	int sock_r;

	sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_r < 0)
	{
		printf("error in socket\n");
		return -1;
	}
	while (1)
		reception_network_packet(sock_r);
	return 0;
}

int main(void)
{
	open_raw_socket();
	return (0);
}
