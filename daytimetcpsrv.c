//#include "unp.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
typedef struct sockaddr SA;

#define MAXLINE 1024
#define LISTENQ 256

int main(int argc,char **argv)
{
	int listenfd,connfd,n;
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	time_t ticks;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
	servaddr.sin_port = htons(9999);
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	listen(listenfd,LISTENQ);
	for(;;){
		connfd = accept(listenfd,(SA*)NULL,NULL);
		ticks = time(NULL);
		snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&ticks));
		for(int i=0;i<strlen(buf);i++)
			write(connfd,&buf[i],1);
		//write(connfd,buf,strlen(buf));
		close(connfd);
	}
}
