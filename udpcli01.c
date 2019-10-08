#include "unp.h"

void dg_cli3(FILE *fp,int sockfd,const SA *pservaddr,socklen_t servlen)
{
	int n;
	char sendline[MAXLINE],recvline[MAXLINE];
	connect(sockfd,(SA *)pservaddr,servlen);
	while(fgets(sendline,MAXLINE,fp)!=NULL){
		write(sockfd,sendline,strlen(sendline));
		n = read(sockfd,recvline,MAXLINE);
		recvline[n] = 0;
		fputs(recvline,stdout);
	}
}

void dg_cli2(FILE *fp,int sockfd,const SA* pservaddr,socklen_t servlen)
{
	int n;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	socklen_t len;
	struct sockaddr *preply_addr;
	preply_addr = malloc(servlen);
	while(fgets(sendline,MAXLINE,fp)!=NULL){
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		len = servlen;
		n = recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
		if(len!=servlen || memcmp(pservaddr,preply_addr,len)!=0){
			printf("reply from %s (ignored)\n",sock_ntop(preply_addr,len));
			continue;
		}
		recvline[n] = 0;
		fputs(recvline,stdout);
	}
}

void dg_cli(FILE *fp,int sockfd,const SA* pservaddr,socklen_t servlen)
{
	int n;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	while(fgets(sendline,MAXLINE,fp)!=NULL){
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
		n = recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
		recvline[n] = 0;
		fputs(recvline,stdout);
	}
}

int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc!=2)
		err_quit("usage: udpcli <IPaddresss>");
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	dg_cli3(stdin,sockfd,(SA*)&servaddr,sizeof(servaddr));
}
