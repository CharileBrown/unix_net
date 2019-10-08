#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
typedef struct sockaddr SA;

int main()
{
	char b = 0;
	int a = 1,c;
	c = a++;
	printf("%ld %ld\n",(long)a,(long)c);
	
}
