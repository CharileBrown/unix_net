#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
typedef struct sockaddr SA;

int main()
{
	char *ip = "127.0.0.1";
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	inet_pton(AF_INET,ip,&servaddr.sin_addr);
	if( connect(sockfd,(SA *)&servaddr,sizeof(servaddr)) <0 )
		printf("error\n");

}
