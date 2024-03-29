#ifndef _MY_ERR_
#define _MY_ERR_
#include "unp.h"


static void err_doit(int errnoflag,int error,const char *fmt,va_list ap)
{
	char buf[MAXLINE];
	vsnprintf(buf,MAXLINE,fmt,ap);
	if(errnoflag)
		snprintf(buf+strlen(buf),MAXLINE-strlen(buf),":%s",strerror(error));
	strcat(buf,"\n");
	fflush(stdout);
	fputs(buf,stderr);
	fflush(NULL);
}

void err_quit(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(0,0,fmt,ap);
	va_end(ap);
	exit(1);
}

void err_sys(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,errno,fmt,ap);
	va_end(ap);
	exit(1);
}
#endif
