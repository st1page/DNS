
#include "udp.hpp"
#include "utils.hpp"
#include "DNSType.h"

#include <stdio.h>

int main(){
	char s[255];
	scanf("%s",s);
	if(!isDomainName(s)){
		printf("%s\n",s);
		puts("not a domain");
		return 0;
	}
	char *t = dn2msg(s);
	DNSPackage package(1,t);
	free(t);

	char *Qmsg = (char*)malloc(package.len());
	package.toString(Qmsg);

	udpClient client = udpClient("8.8.8.8", 53);
	client.send(Qmsg, package.len());

	char recv_buff[520];
	client.recv(recv_buff, 512);
	package = DNSPackage(recv_buff);
	printf("%d %d\n",package.header.ANCOUNT,package.header.QDCOUNT);
	for(int i = 0;i < package.header.ANCOUNT;i++){
		printf("%s\n",package.answers[i].Data);
	}
	return 0;
}