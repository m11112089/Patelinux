#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <linux/udp.h>
#include "global.h"

#define BUF_SIZE	ETH_FRAME_LEN
#define HEADER_OVERHEAD		ETH_HLEN + IP4_HDRLEN + UDP_HDRLEN
#define PAYLOAD_LENGTH	1472
void usage()
{
	printf("usage:\n");
	printf("\tmandatory:\n");
	printf("\t\t-s source interface name\n");
	printf("\t\t-d destination interface name\n");
	printf("\toptinal:\n");
	printf("\t\t-c numer of count for loop test, 0 means forever[default 5]\n");
	printf("\t\t-i interval in millisecond [default: no delay]\n");
	printf("\t\t-v Validate received data integrity\n");
	printf("\t\t-b Debug enable received data integrity\n");
	printf("\t\t-D Duration for single test\n");
	exit(1);
}	

uint8_t send_data[ETH_FRAME_LEN - ETH_HLEN - IP4_HDRLEN - UDP_HDRLEN];
char sendbuf[BUF_SIZ];
unsigned char buffer[BUF_SIZE];
int main(int argc, char *argv[])
{
	int sockfd, sockfd_src, sockfd_dest;
	struct ifreq if_idx;
	struct ifreq if_mac, if_addr;
	int tx_len = 0;
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct ip *iphdr = (struct ip *) (sendbuf + sizeof(struct ether_header));
	struct udphdr *udphdr = (struct udphdr *) (sendbuf + sizeof(struct ether_header) + sizeof(struct iphdr));	
	uint8_t *payload = sendbuf + HEADER_OVERHEAD;
	struct sockaddr_ll socket_address;
	char *s_ifName=NULL;
	char *d_ifName=NULL;
	int status;
	int datalen=sizeof(send_data);
	struct in_addr src_ip, dst_ip;
	uint8_t dst_mac[6];
    int opt;
    int interval=0;
	int forever = 0;
	int debug = 0;
	int validate = 0;
	int duration = 0;
	time_t begin;
	int time_up;
		
	/*parameters for receive*/
	int portno, buflen;
	int ifname_len;
	unsigned short iphdrlen;
	struct sockaddr_in src_addr, dest_addr;
	struct sockaddr saddr;
	int saddr_len = sizeof (saddr);
	struct ethhdr *eth = (struct ethhdr *)(buffer);	
	struct iphdr *ip = (struct iphdr*)(buffer + ETH_HLEN);
	struct udphdr *udp;
	unsigned char *data;
	int count=0, loop_count=5, success_count=0;
	struct timeval tv;
	
	while ((opt = getopt (argc, argv, "s:d:c:i:bvD:")) != -1)
	{
		switch (opt)
		{
			case 's':
				s_ifName = optarg;
				break;
			case 'd':
				d_ifName = optarg;
				break;
			case 'c':
				loop_count = atoi(optarg);
				if(loop_count == 0)
					forever = 1;
				break;
			case 'i':
				interval = atoi(optarg);
				break;
			case 'D':
				duration = atoi(optarg);
				if(duration)
				{
					loop_count = 0;
					forever = 1;
				}
				break;
			case 'b':
				debug = 1;
				break;
 			case 'v':
				validate = 1;
				break;
          default: /* '?' */
            usage (argv[0]);
		}
	}

	if(s_ifName == NULL)
#ifndef USE_BCAST
		if(d_ifName == NULL) 
#endif
		usage();
 
	/* ------------- Sourece ------------- */
	/* Open RAW socket to send on */
	if ((sockfd_src = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, s_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_src, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, s_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_src, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
	memset(&if_addr, 0, sizeof(struct ifreq));
	strncpy(if_addr.ifr_name, s_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_src, SIOCGIFADDR, &if_addr) < 0)
	    perror("SIOCGIFAFFR");
	
	sockfd_dest=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	if(sockfd_dest<0)
		error("error in socket\n");
	
	ifname_len = strnlen(s_ifName, IFNAMSIZ);
 	setsockopt(sockfd_dest, SOL_PACKET, SO_BINDTODEVICE, s_ifName, ifname_len);

	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, s_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_dest, SIOCGIFINDEX, &if_idx) < 0)
	    error("SIOCGIFINDEX");
	
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, s_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_dest, SIOCGIFHWADDR, &if_mac) < 0)
	    error("SIOCGIFHWADDR");

#ifdef USE_BCAST
	dst_ip.s_addr = htonl(INADDR_BROADCAST);
	dst_mac[0] = BCAST_MAC0;
	dst_mac[1] = BCAST_MAC1;
	dst_mac[2] = BCAST_MAC2;
	dst_mac[3] = BCAST_MAC3;
	dst_mac[4] = BCAST_MAC4;
	dst_mac[5] = BCAST_MAC5;
#else
{	
	struct ifreq if_req;
	
	/* Get the MAC address of the interface to send on */
	memset(&if_req, 0, sizeof(struct ifreq));
	strncpy(if_req.ifr_name, d_ifName, IFNAMSIZ-1);
	if (ioctl(sockfd_src, SIOCGIFHWADDR, &if_req) < 0)
	    error("SIOCGIFHWADDR");	
	memcpy(dst_mac, &if_req.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);

	if (ioctl(sockfd_dest, SIOCGIFADDR, &if_req) < 0)
	    error("SIOCGIFADDR");	
	dst_ip =  ((struct sockaddr_in*)&if_req.ifr_addr)->sin_addr;	
}	
#endif
	
	/* Construct the Ethernet header */
	memset(sendbuf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = dst_mac[0];
	eh->ether_dhost[1] = dst_mac[1];
	eh->ether_dhost[2] = dst_mac[2];
	eh->ether_dhost[3] = dst_mac[3];
	eh->ether_dhost[4] = dst_mac[4];
	eh->ether_dhost[5] = dst_mac[5];
	/* Ethertype field */
	eh->ether_type = htons(ETH_P_IP);
	tx_len += sizeof(struct ether_header) + IP4_HDRLEN + UDP_HDRLEN + PAYLOAD_LENGTH;

	// IPv4 header
	iphdr->ip_hl = IP4_HDRLEN / sizeof (uint32_t);
	iphdr->ip_v = 4;
	iphdr->ip_tos = 0;
	iphdr->ip_len = htons (IP4_HDRLEN + UDP_HDRLEN + datalen);
	iphdr->ip_id = htons (0);
	iphdr->ip_off = htons ((0 << 15)
					  + (1 << 14)
					  + (0 << 13)
					  +  0);
	iphdr->ip_ttl = 64;
	iphdr->ip_p = IPPROTO_UDP;

	iphdr->ip_src = ((struct sockaddr_in*)&if_addr.ifr_addr)->sin_addr;
	iphdr->ip_dst = dst_ip;

	iphdr->ip_sum = 0;
	iphdr->ip_sum = checksum ((uint16_t *) iphdr, IP4_HDRLEN);

	// UDP header
	udphdr->source = htons (UDP_CLIENTPORT);
	udphdr->dest = htons (UDP_SERVER_PORT);
	udphdr->len = htons (UDP_HDRLEN + PAYLOAD_LENGTH);
	udphdr->check = udp4_checksum (*iphdr, *udphdr, send_data, PAYLOAD_LENGTH);
	//memcpy (payload, send_data, datalen);
	gen_payload(payload, PAYLOAD_LENGTH);

	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = dst_mac[0];
	socket_address.sll_addr[1] = dst_mac[1];
	socket_address.sll_addr[2] = dst_mac[2];
	socket_address.sll_addr[3] = dst_mac[3];
	socket_address.sll_addr[4] = dst_mac[4];
	socket_address.sll_addr[5] = dst_mac[5];
	
	if(duration)
		time(&begin);
		
	while( forever || (count < loop_count) )
	{
		time_t start, end;
		double elapsed;
		int timeout = 0;
	
		count++;		
		if(debug)
			printf("Loop(%i) total(%i)\n", count, loop_count);
	
		if (sendto(sockfd_src, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
			error("Send failed\n");

		portno = UDP_CLIENTPORT;
	
		time(&start);
		while(1)
		{
			time(&end);
			
			if(duration && (difftime(end, begin) > duration))
			{
					count -= 1;
					forever = 0;
					break;
			}	
			elapsed = difftime(end, start);				
			if (elapsed>=5)
			{
				timeout = 1;
				printf("Time out!!! No echo packet received\n"); 
				break;
			}

			memset(buffer,0,BUF_SIZE);	

			/* Set receive timeout */
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			setsockopt(sockfd_dest, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
			
			buflen=recvfrom(sockfd_dest,buffer,BUF_SIZE,0,&saddr,(socklen_t *)&saddr_len);
			if(buflen<=0)
				continue; /*timeout*/

			printf("%d bytes received\n", buflen);
			iphdrlen = ip->ihl*4;
			udp=(struct udphdr*)(buffer + iphdrlen + ETH_HLEN);

			if( (ip->protocol != IPPROTO_UDP) || (ntohs(udp->dest) != portno) )
				continue;

			if(validate)
			{
				uint8_t *rcv_data = ((uint8_t *)udp) + 8;
				//printf("%02x:%02x:%02x:%02x==%02x:%02x:%02x:%02x ==>%d\n", payload[0], payload[1], payload[2], payload[3], rcv_data[0], rcv_data[1], rcv_data[2], rcv_data[3], sizeof(send_data));
				for(int i=0; i<PAYLOAD_LENGTH; i++)
				{
					if(payload[i] != rcv_data[i])
					{
						printf("incurect in %d %02x:%02x\n", i, payload[i], rcv_data[i]);
						break;
					} 
				}
				//if(memcmp(payload, rcv_data, 1472) == 0)
				//	success_count++;
			} else
				success_count++;					 					
			break;
		}
		if( interval && (timeout == 0) )
			usleep(interval * 1000);
	}
	if (success_count != count)
	{
		printf("Fail(%d:%d)\n", count, success_count);
		return -1;
	}

	printf("Pass(%d:%d)\n", count, success_count);
	return 0;
}
