#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string.h>
#include <stdlib.h>

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
char* dn2msg(char *dn){
	puts("aaa");
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
#endif