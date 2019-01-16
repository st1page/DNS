#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "stdio.h"
bool isDomainName(char *dn){
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
// "www.google.com" -> "3www6google3com"
// "www.google.com." -> "3www6google3com"
char* dn2msg(char *dn){
	char *msg;
	size_t len = strlen(dn);
	if(dn[len-1]!='.') {
		msg = (char*)malloc(len+2);
		msg[len+1] = 0;
	}
	else {
		msg = (char*)malloc(len+1);
		msg[len] = 0;
	}
	size_t l,r;
	for(size_t i=0;i<len;i=r+1){
		l = i; r = i;
		while(dn[r]!='.'&&r!=len) r++;
		msg[l] = r-l;
		for(size_t j=l;j<r;j++) msg[j+1] = dn[j];
	}
	return msg;
}
char* msg2dn(const char *msg){
	char *dn;
	size_t len = strlen(msg);
	dn = (char*)malloc(len);
	size_t l,r;
	for(size_t i=0;i<len;i=r+1){
		l = i; r = i+msg[i];
		for(size_t j=l;j<r;j++) dn[j] = msg[j+1];
		dn[r] = '.';
	}
	dn[len-1] = 0;
	return dn;
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

namespace dns{
	inline bool is_ptr(const unsigned char &c){
		return (c>>6)==0x3;
	}
	inline size_t ptr_offset(const char *c){
		return ((size_t)(*c&0xf)<<8) + (size_t)(*(c+1))&0xff;
	}
	size_t dn_filed_len(const char *cur){
		size_t len =0;
		while(*cur){
			if(is_ptr(*cur)) {
				len += 2;
				break;
			}
			len += *cur + 1;
			cur += *cur + 1;
		}
		return len;
	}
	size_t dn_len(const char *cur, const char *begin){
		size_t len =0;
		while(*cur){
			while(is_ptr(*cur)) cur = begin + ptr_offset(cur);
			len += *cur + 1;
			cur += *cur + 1;
		}
		return len;
	}
	char *full_dn(const char *cur, const char *begin){
		size_t len = dn_len(cur, begin);
		char *res = (char*)malloc(len+1);
		char *dst = res;
		while(*cur){
			while(is_ptr(*cur)) cur = begin + ptr_offset(cur);
			memcpy(dst,cur,*cur+1);
			dst += *cur + 1;
			cur += *cur + 1;
		}
		*dst = 0;
		return res;
	}
}
#endif