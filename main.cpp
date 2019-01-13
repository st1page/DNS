#include <cstdio>

#include "utils.hpp"

int main(){
	char s[255];
	scanf("%s",s);
	if(!isDomainName(s)){
		printf("%s\n",s);
		puts("not a domain");
	}
	struct sockaddr addr;
	int fd = udp_init("8.8.8.8", 53, (sockaddr_in*)&addr);

	char *package_p;
	size_t package_len = encodeQ(s, &package_p);
	sendto(fd, (char *)package_p, package_len, 0, &addr, sizeof(addr));
	char buf[512];
	socklen_t len;
	int ret = recvfrom(fd, buf, 512, 0, &addr, &len);
	if(!ret) {
		puts("not recive"); return 0;
	}
	DNSHeader *p = (DNSHeader *)buf;
	printf("%d\n%d\n",htons(p->Questions),htons(p->AnswerRRs));
	

}