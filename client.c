#include <stdio.h>     /* perror */
#include <stdlib.h>    /* exit	*/
#include <sys/types.h> /* WNOHANG */
#include <sys/wait.h>  /* waitpid */
#include <string.h>    /* memset */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h> /* gethostbyname */

#define true 1
#define false 0

#define PORT 3490       /* Server的端口 */
#define MAXDATASIZE 100 /* 一次可以读的最大字节数 */

int main(int argc, char *argv[])
{
    int sockfd;
    int numbytes;
    // char buf[MAXDATASIZE],send[MAXDATASIZE];          //save
    struct hostent *he;            
    /* 主机信息 
    	struct hostent
	{
		char *h_name;         //正式主机名
		char **h_aliases;     //主机别名
		int h_addrtype;       //主机IP地址类型：IPV4-AF_INET
		int h_length;		  //主机IP地址字节长度，对于IPv4是四字节，即32位
		char **h_addr_list;	  //主机的IP地址列表
	};
	#define h_addr h_addr_list[0]   //保存的是IP地址
    */
    struct sockaddr_in server_addr; 
    /* 对方地址信息 
    struct sockaddr {  
     unsigned short sa_family;
　　 char sa_data[14];                  
　　 }; 
    sa_family是通信类型，最常用的值是 "AF_INET"
    sa_data14字节，包含套接字中的目标地址和端口信息
    */
    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    /* get the host info */
    if ((he = gethostbyname(argv[1])) == NULL)
    {
        /* 注意：获取DNS信息时，显示出错需要用herror而不是perror */
        /* herror 在新的版本中会出现警告，已经建议不要使用了 */
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); /* short, NBO */
    server_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    memset(&(server_addr.sin_zero), 0, 8); /* 其余部分设成0 */

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }

    // if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    // {
    //     perror("recv");
    //     exit(1);
    // }

    char sendbuffer[MAXDATASIZE];
    char recvbuffer[MAXDATASIZE];
    while (fgets(sendbuffer, sizeof(sendbuffer), stdin) != NULL)
   {
         send(sockfd, sendbuffer, strlen(sendbuffer),0); ///发送
         if(strcmp(sendbuffer,"exit\n")==0)
             break;
         recv(sockfd, recvbuffer, sizeof(recvbuffer),0); ///接收
         fputs(recvbuffer, stdout);
 
         memset(sendbuffer, 0, sizeof(sendbuffer));
         memset(recvbuffer, 0, sizeof(recvbuffer));
   }
    
    // buf[numbytes] = '\0';
    // printf("Received: %s", buf);
    close(sockfd);

    return true;
}
