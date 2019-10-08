#ifndef _UNP_
#define _UNP_

#define LISTENQ 8
#define MAXLINE 1024
#define max(x,y) ((x)>(y)?(x):(y))
#define SERV_PORT 9999

typedef struct sockaddr SA;


#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>  
#include "myIO.h"
#include "my_err.h"

typedef void sigfunc(int);

sigfunc *signal(int signo,sigfunc *func)
{
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if( signo == SIGALRM ){
	#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
	#endif
	}else{
	#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
	#endif	
	}
	if( sigaction(signo,&act,&oact) < 0 )
		return SIG_ERR;
	return oact.sa_handler;

}


char *sock_ntop(const struct sockaddr *sa,socklen_t addrlen)
{
	char portstr[8];
	static char str[128];
	switch(sa->sa_family){
	case AF_INET:{
		struct sockaddr_in *sin = (struct sockaddr_in *)sa;
   		if( inet_ntop(AF_INET,&sin->sin_addr,str,sizeof(str)) == NULL )
			return NULL;
		if(ntohs(sin->sin_port)!=0){
			snprintf(portstr,sizeof(portstr),":%d",ntohs(sin->sin_port));
			strcat(str,portstr);
		}
		return str;
	     }
	case AF_INET6:{
		struct sockaddr_in6 *sin = (struct sockaddr_in6 *)sa;
		if(inet_ntop(AF_INET6,&sin->sin6_addr,str,sizeof(str)) == NULL)
			return NULL;
		if(ntohs(sin->sin6_port)!=0){
			snprintf(portstr,sizeof(portstr),"%d",ntohs(sin->sin6_port));
			strcat(str,portstr);
		}
		return str;
	}
}
}

#endif
