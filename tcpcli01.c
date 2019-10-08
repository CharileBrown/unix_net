#include "unp.h"

#define SERV_PORT 9999

struct args{
	long arg1;
	long arg2;
};

struct result{
	long sum;
};

void str_cli4(FILE *fp,int sockfd)
{
	int maxfdp1,stdineof;
	fd_set rset;
	char buf[MAXLINE];
	int n;

	stdineof = 0;
	FD_ZERO(&rset);
	for(;;){
		if(stdineof==0)
			FD_SET(fileno(fp),&rset);
		FD_SET(sockfd,&rset);
		maxfdp1 = max(fileno(fp),sockfd) + 1;	
		select(maxfdp1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(sockfd,&rset)){
			if((n = read(sockfd,buf,MAXLINE)) == 0){
				if(stdineof == 1)
					return;
				else
					err_quit("str_cli:server terminated prematurely");
			}
			write(fileno(stdout),buf,n);
		}
		if(FD_ISSET(fileno(fp),&rset)){
			if((n = read(fileno(fp),buf,MAXLINE )) == 0){
				stdineof = 1;
				shutdown(sockfd,SHUT_WR);
				FD_CLR(fileno(fp),&rset);
				continue;
			}
			writen(sockfd,buf,n);
		}
	}

}

void str_cli3(FILE *fp,int sockfd)
{
	int maxfdp1;
	fd_set rset;
	char sendline[MAXLINE],recvline[MAXLINE];

	FD_ZERO(&rset);
	for(;;){
		FD_SET(fileno(fp),&rset);
		FD_SET(sockfd,&rset);
		maxfdp1 = max(fileno(fp),sockfd) + 1;
		select(maxfdp1,&rset,NULL,NULL,NULL);
		if( FD_ISSET(sockfd,&rset) ){
			if( readline(sockfd,recvline,MAXLINE) == 0 )
				err_quit("str_cli:server terminated prematurely");
			fputs(recvline,stdout);
		}
		if( FD_ISSET(fileno(fp),&rset) ){
			if( fgets(sendline,MAXLINE,fp) == NULL )
				return;
			writen(sockfd,sendline,strlen(sendline));
		}
	}

}
void str_cli2(FILE *fp,int sockfd)
{
	char sendline[MAXLINE];
	struct args args;
	struct result result;
	while(fgets(sendline,MAXLINE,fp)!=NULL){
		if( sscanf(sendline,"%ld %ld",&args.arg1,&args.arg2) != 2 ){
			printf("invalid input:%s\n",sendline);
			continue;
		}
		writen(sockfd,&args,sizeof(args));
		if( readn(sockfd,&result,sizeof(result)) == 0)
			err_quit("str_cli:server terminated prematurely");
		printf("%ld\n",result.sum);
	}
}

void str_cli(FILE *fp,int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	while(fgets(sendline,MAXLINE,fp) != NULL){
		writen(sockfd,sendline,strlen(sendline));
		//writen(sockfd,"a",1);
		//sleep(10);
		//writen(sockfd,sendline,strlen(sendline));
		if( readline(sockfd,recvline,MAXLINE) ==0 )
			err_quit("str_cli: server terminated prematurely");
		fputs(recvline,stdout);
	}
}

int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 2)
		err_quit("usage: tcpcli <IPaddresss>");
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	connect(sockfd,(SA *)&servaddr,sizeof(servaddr));
	str_cli(stdin,sockfd);
	exit(0);
}
