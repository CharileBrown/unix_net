#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
 
#define MAXLINE 4096
 
int main(int argc, char** argv) {
	int   sockfd, n;
	char recvLine[MAXLINE], sendLine[MAXLINE];
	struct sockaddr_in servaddr;
 
	if (argc != 2) {   //命令行参数为要连接的服务端IP地址
		printf("Usage: ./Client <ip_address>\n");
		return 0;
	}
 
	if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
 
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6666);
 
	if ( inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0 ) {
		printf("inet_pton error for %s\n", argv[1]);
		return 0;
	}
 
	if (connect(sockfd,(struct sockaddr*) & servaddr,sizeof(servaddr)) < 0 ) {
		//客户端需要调用connect()连接服务器，connect和bind的参数形式一致，区别在于bind的参数是自己的地址，而connect的参数是对方的地址。connect()成功返回0，出错返回-1
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
 
	printf("send msg to server: \n");
	while(1){
		fgets(sendLine,sizeof(sendLine),stdin);
 
		if ( send(sockfd,sendLine,sizeof(sendLine),0) < 0 ) {
			printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
			return 0;
		}
		if (strcmp(sendLine, "exit\n") == 0)
			break;
		memset(sendLine, 0, sizeof(sendLine));
	}
	
	close(sockfd);
	return 0;
}

