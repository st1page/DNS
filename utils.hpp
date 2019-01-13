#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "DNSHeader.h"

bool isDomainName(const char *dn){
	size_t len = strlen(dn);
	if(len == 0 || len > 255 ||
		dn[0]=='.' || dn[0]=='-' ||
		dn[len - 1]=='-') return false;
	size_t l,r; 
	for(size_t i=0;i<len;i=r+1){
		l = i; r = i;
		while(dn[r]!='.' && r!=len) r++;
		if(r-l == 0 || r-l > 63) return false;
	}
	return true;
}

char* dn2msg(const char *dn){
	char *msg;
	size_t len = strlen(dn);
	if(dn[len-1]!='.') msg = (char*)calloc(len+2,1);
	else msg = (char*)calloc(len+1,1);
	size_t l,r;
	for(size_t i=0;i<len;i=r+1){
		l = i; r = i;
		while(dn[r]!='.'&&r!=len) r++;
		msg[l] = r-l;
		for(size_t j=l;j<r;j++) msg[j+1] = dn[j];
	}
	return msg;
}

int udp_init(const char *ip,
			 const unsigned short port,
			 struct sockaddr_in *addr_p){
	memset(addr_p, 0, sizeof(struct sockaddr));
	int client_fd;

	client_fd = socket(AF_INET, SOCK_DGRAM, 0);

	addr_p->sin_family = AF_INET;
	addr_p->sin_addr.s_addr = inet_addr(ip);
	addr_p->sin_port = htons(port);
	return client_fd;
}

/********************************************************************************/
size_t encodeQ(char *__dn,char **res){
	char *dn = dn2msg(__dn);
	size_t dn_len = strlen(dn);
	DNSHeader *package = (DNSHeader *)malloc(sizeof(DNSHeader)+dn_len+1+4);
	package->RD = htons(0x1);
	package->Questions = htons(0x1);
	char *p8 = (char *)package + sizeof(DNSHeader);
	memcpy(p8, dn, dn_len+1);
	uint16_t *p16 = (uint16_t *)(p8 + dn_len + 1);
	*p16 = htons(0x1);
	++p16;
	*p16 = htons(0x1);
	*res = (char *)package;
	return sizeof(DNSHeader)+dn_len+1+4;
}
void decodeR(){

}
#endif
