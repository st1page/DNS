#ifndef _MESSENGER_HPP_
#define _MESSENGER_HPP_

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class udpClient{
private:
	struct sockaddr dst_addr;
	int fd;
public:
	udpClient(const char *ip, const unsigned short port){
		fd = socket(AF_INET, SOCK_DGRAM, 0);

		struct sockaddr_in *addr_p = (struct sockaddr_in *)&dst_addr;
		memset(addr_p, 0, sizeof(struct sockaddr));
		addr_p->sin_family = AF_INET;
		addr_p->sin_addr.s_addr = inet_addr(ip);
		addr_p->sin_port = htons(port);
	}
	~udpClient(){
		close(fd);
	}
	void send(const void *buf, size_t len){
		sendto(fd, buf, len, 0, &dst_addr, (socklen_t)sizeof(dst_addr));
	}
	ssize_t recv(void *buf, size_t len){
		struct sockaddr_in src; 
		socklen_t src_len;
		return recvfrom(fd, buf, len, 0, (struct sockaddr*)&src, &src_len);
	}
};

class udpServer{
private:

public:
	udpServer(){

	}
};
#endif