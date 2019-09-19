#include<stdio.h>
#include<stdlib.h>
#include<string.h>      
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
 
#define MAXLINE 4096
 
/**************程序流程*******************
1 用socket()函数创建一个socket
2 用bind()绑定到一个本地的地址，这样其他的socket可以用connect()连接上去
3 用listen()指出愿意接收连接并指定进来的连接的队列限制
4 用accept()函数来接收连接
*****************************************/
 
int main(int argc, char const *argv[])
{
    int listenfd; //监听套接字描述符，管理客户端连接到服务端的个数
    int connfd;   //连接套接字描述符，被send()和recv()函数所使用
    struct sockaddr_in servaddr; //套接字地址结构体
    //结构体具体定义见sockaddr_in.jpg
    char buff[MAXLINE];
    int n;
    if((listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1){ 
        //socket函数向系统申请一个通信端口 原型：int socket(int domain, int type, int protocol);
        //domain@ PF_INET, AF_INET： Ipv4网络协议； PF_INET6, AF_INET6： Ipv6网络协议
        //type@ SOCK_STREAM： 提供面向连接的稳定数据传输，即TCP协议。SOCK_DGRAM： 使用不连续不可靠的数据包连接。
        //protocal@ 传输协议编号，一般设置为0就好。
 
        printf("create socket error: %s(errno:%d)\n",strerror(errno),errno);
        //errno 为全局变量，包含在<error.h>中，程序出错时自动填充。错误号的源代码定义：https://www.cnblogs.com/xrcun/p/3210889.html
        //strerror()函数不是线程安全的，strerror_r()是线程安全的。
 
        return 0;
    }
    memset(&servaddr,0,sizeof(servaddr)); //初始化结构体
    servaddr.sin_family = AF_INET;   //设置地址家族，此处为IPv4
 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
        //设置地址：servaddr.sin_addr.s_addr是ip地址，
        //htonl()的作用：将主机的unsigned long值转换成网络字节顺序（32位）；作用相反的函数即把网络字节顺序转化成主机序列为ntohl()函数；htons()就是转化为short类型的（16位两字节）
        //机器上可能有多块网卡，也就有多个IP地址，这时候你要选择绑定在哪个IP上面，如果指定为INADDR_ANY，那么系统将绑定默认的网卡【即IP地址】
        //servaddr.sin_addr.s_addr = inet_addr("192.168.81.130")手动设置服务器的IP地址；inet_addr()作用是将一个IP字符串转化为一个网络字节序的整数值，inet_ntoa()作用与其相反
    servaddr.sin_port = htons(6666);  //设置端口
 
    if(bind(listenfd,(struct sockaddr*) &servaddr , sizeof(servaddr)) == -1){
        //sockaddr的缺陷是：把目标地址和端口信息混在一起了; sockaddr_in解决了sockaddr的缺陷,但是参数传递时需要类型转换为sockaddr。
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
 
    if(listen(listenfd,10) == -1){
        //监听端口后，被动套接字会等待连接，此时系统维护着两个队列，第一个队列存放的是已建立连接的套接字（即完成三次握手后的），第二个队列存放的是未建立连接的套接字（处在三次握手中的）。每次accept函数会将第一个队列中的套接字返回，进行通信，以完成网络传输。
        //所以listen的第二个参数意味着这两个队列的总和大小
        printf("listen socket error:%s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
 
    printf("========== waiting for client's request =============\n");
 
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
 
    while(1){
        if( connfd == -1){
            //等待客户端连接，连接将加入到等待接受(accept())的队列中
            //第一个参数为本地(服务端监听描述符)，第二个参数存有客户端IP地址和端口号，第三个参数为sockaddr*指向区域的长度
            //accept在有客户端连接上来时会被阻塞
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd,buff,MAXLINE,0);
        //buff:缓冲区名字 MAXLINE:缓冲区大小 最后一个参数(flags):
        //通常flags设置为0，此时recv()函数读取tcp 缓冲区中的数据到buf中，并从tcp 缓冲区中移除已读取的数据，
        //如果把flags设置为MSG_PEEK，仅仅是把tcp 缓冲区中的数据读取到buf中，没有把已读取的数据从tcp 缓冲区中移除，如果再次调用recv()函数仍然可以读到刚才读到的数据。
 
        buff[n] = '\0'; 
        //scanf之类的字符串处理函数会自动将缓冲区的最后一字节设置为 '\0',但是诸如read()等函数就不会自动设置
        //TCP缓冲区大小一般分为三级，详见 https://blog.csdn.net/nawenqiang/article/details/81503870
 
        printf("recv msg from client: %s\n",buff);
		memset(buff,0,sizeof(buff)/sizeof(char) ); //发送完一次后清空缓冲区，为下一次发送做好准备。
    }
	close(connfd);
    close(listenfd);
 
    return 0;
}

