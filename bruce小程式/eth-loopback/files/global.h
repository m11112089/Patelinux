#ifndef _ETHER_TESTER_H
#define _ETHER_TESTER_H

#define USE_BCAST
#define BUF_SIZ				1024
#define UDP_CLIENTPORT     	50002
#define UDP_SERVER_PORT    	50002
#define IP4_HDRLEN 			20
#define UDP_HDRLEN  		8

#define BCAST_MAC0			0xFF
#define BCAST_MAC1			0xFF
#define BCAST_MAC2			0xFF
#define BCAST_MAC3			0xFF
#define BCAST_MAC4			0xFF
#define BCAST_MAC5			0xFF

extern uint16_t checksum (uint16_t *addr, int len);
extern uint16_t udp4_checksum (struct ip iphdr, struct udphdr udphdr, uint8_t *payload, int payloadlen);
extern void error(char *msg);
extern void gen_payload(uint8_t *pdata, int length);
#endif /*_ETHER_TESTER_H*/
