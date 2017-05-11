#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <errno.h>

#include "person.pb.h"

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		printf("usage:	./client <ip>\n");
		return -1;
	}
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if ( -1 != fd ) {
		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8000);//设置的端口为8000 
//		addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
		int ret = inet_pton(AF_INET, argv[1], &addr.sin_addr);
		printf("client,fd:%d,prot:%d,addr:%d\n", fd, addr.sin_port, addr.sin_addr.s_addr);
		if ( 1 == ret ) {
			if ( !connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) ) {
#if 0
				const char *data = "hi server, can you hear me?";
				send(fd, data, strlen(data), 0);
#else
				std::string data;
				person::Person p;
				p.set_id(1);
				p.set_name("eddy");
//				p.set_email("123@eddy.com");
			   	p.SerializeToString(&data);
				ssize_t ret = send(fd, data.data(), data.length(), 0);
				printf("client,send data,size:%d,data len:%d\n", ret, data.length());
#endif
				char ack[256] = {0};
				if ( recv(fd, ack, 256, 0) > 0 ) {
					printf("client,recv data:%s\n", ack);
				}
			} else {
				printf("client,connect failed,errno:%s\n", strerror(errno));
			}
		} else {
			printf("client,inet_pton failed,ret:%d,errno:%s\n", ret, strerror(errno));
		}
		::close(fd);
		fd = -1;
	} else {
		printf("client,socket failed,errno:%s\n", strerror(errno));
	}
	return 0;
}
