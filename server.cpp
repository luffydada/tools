#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "person.pb.h"

int main(int argc, char *argv[])
{
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if ( -1 != fd ) {
		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8000);//设置的端口为8000 
		addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址
		printf("server,fd:%d,port:%d,addr:%d\n", fd, addr.sin_port, addr.sin_addr.s_addr);
		if ( -1 != ::bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) ) {
			if ( !listen(fd, 10) ) {
//				int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
				int client_fd = -1;
				char data[256] = {0};
				struct sockaddr_in in_addr = {0};
				socklen_t len = sizeof(in_addr);
				while ( 1 ) {
					if ( -1 == (client_fd = accept(fd, (struct sockaddr *)&in_addr, &len)) ) {
						printf("server,accept failed,errno:%s\n", strerror(errno));
						continue;
					}
					printf("server,connect client,len:%d,ip:%s,port:%d\n", len, inet_ntoa(in_addr.sin_addr), ntohs(in_addr.sin_port));
					ssize_t ret = recv(client_fd, data, 256, 0);
					if ( ret > 0 ) {
#if 0
						printf("server,recv data:%s\n", data);
#else
						std::string info(data);
						person::Person b;
						b.ParseFromString(info);
						printf("server,recv data,len:%d,id:%d,name:%s,email:%s\n", ret, b.id(), b.name().data(), b.email().data());
#endif
						const char *ack = "hi,i have recv your data";
						send(client_fd, ack, strlen(ack), 0);
					}
				}
			} else {
				printf("server,listen failed,errno:%s\n", strerror(errno));
			}
		} else {
			printf("server,bind failed,errno:%s\n", strerror(errno));
		}
		::close(fd);
		fd = -1;
	} else {
		printf("server,socket failed,errno:%s\n", strerror(errno));
	}
	return 0;
}

