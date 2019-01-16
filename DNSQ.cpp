
#include "udp.hpp"
#include "utils.hpp"
#include "DNSType.h"

#include <stdio.h>

int main(){
	puts("input a domain name");
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
	ssize_t ret = client.recv(recv_buff, 512);
	if(ret == -1){
		puts("not recv!");
		return 0;
	}
	DNSPackage rpackage(recv_buff);
	printf("\n%d %d\n",rpackage.header.ANCOUNT,rpackage.header.QDCOUNT);
	for(int i = 0;i < rpackage.header.ANCOUNT;i++){
		rpackage.answers[i].print();
	}
	return 0;
}