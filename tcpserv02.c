#include "unp.h"


#define SERV_PORT 9999

struct args{
	long arg1;
	long arg2;
};

struct result{
	long sum;
};

void str_echo2(int sockfd)
{
	ssize_t n;
	struct args args;
	struct result result;
	for(;;){
		if((n=readn(sockfd,&args,sizeof(args)))==0)
			return;
		result.sum = args.arg1+args.arg2;
		writen(sockfd,&result,sizeof(result));
	}
}


void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while( (pid = waitpid(-1,&stat,WNOHANG)) > 0 )
		printf("child %d terminated\n",pid);
	return;
}

void str_echo(int sockfd)
{
	printf("I am in\n");
	ssize_t n;
	char buf[MAXLINE];
	while(1){
		while( (n=read(sockfd,buf,MAXLINE)) > 0 )
			writen(sockfd,buf,n);
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
	signal(SIGCHLD,sig_chld);
	for(;;){
		clilen = sizeof(cliaddr);
		if((connfd = accept(listenfd,(SA *)&cliaddr,&clilen))<0){
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}
		if( (childpid = fork()) == 0 ){
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	
	}

}
