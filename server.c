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

#define MYPORT 3490 /* 监听的端口 */
#define BACKLOG 10  /* listen的请求接收队列长度 */
#define MAXSIZE 100
int main()
{
    int sockfd, new_fd;            /* 监听端口，数据端口 */
    struct sockaddr_in sa;         /* 自身的地址信息 */
    struct sockaddr_in their_addr; /* 连接对方的地址信息 */
    unsigned int sin_size;
    char buffer[MAXSIZE];

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    sa.sin_family = AF_INET;
    sa.sin_port = htons(MYPORT);     /* 网络字节顺序 */
    sa.sin_addr.s_addr = INADDR_ANY; /* 自动填本机IP */
    memset(&(sa.sin_zero), 0, 8);    /* 其余部分置0 */

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd,
                        (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1){
            perror("accept");
            // continue;
        }
        printf("Got connection from %s\n",
               inet_ntoa(their_addr.sin_addr));

    /* 主循环 */
    while (1)
    {
        memset(buffer,0,sizeof(buffer));
         int len = recv(new_fd, buffer, sizeof(buffer),0);
         if(strcmp(buffer,"exit\n")==0){
             break;
         }
         fputs(buffer, stdout);
         if(send(new_fd, "Hello!\n",sizeof("Hello!\n") , 0)==-1){
             perror("send");
         }
        }
         
    close(sockfd);
    return true;
}
