#include "unp.h"


#define SERV_PORT 9999

void str_echo(int sockfd)
{
	printf("I am in\n");
	ssize_t n;
	char buf[MAXLINE];
	while(1){
		while( (n=read(sockfd,buf,MAXLINE)) > 0 )
			printf("ok\n"),writen(sockfd,buf,n);
		if(n<0&&errno==EINTR)
			continue;
		else if(n<0)
			err_sys("str_echo: read_error");
		break;
	}
}


int main(int argc,char **argv)
{
	int listenfd,connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	bind(listenfd,(SA *)&servaddr,sizeof(servaddr));
	listen(listenfd,LISTENQ);
	for(;;){
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd,(SA *)&cliaddr,&clilen);
		if( (childpid = fork()) == 0 ){
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	
	}

}
