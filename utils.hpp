#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string.h>

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
#endif