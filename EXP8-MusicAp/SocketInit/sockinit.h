#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
// #include "DataBase/user.h"
int CreateSocket(){

    int sock_listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    return sock_listen;
}
/*
    init socket addr is INADDR_ANY, port...
    return 1 == init successful
    return 0 == init fail
*/
int SocketServerInit(struct sockaddr_in *myaddr,int addr,int port){

    int sock_listen = CreateSocket();
    if(sock_listen == -1){
        perror("sock error\n");
        exit(-1);
    }
    //struct sockaddr_in myaddr;
    (*myaddr).sin_family = AF_INET;
    (*myaddr).sin_addr.s_addr = addr;
    (*myaddr).sin_port = htons(port);
    if(bind(sock_listen,(struct sockaddr*)myaddr,sizeof(*myaddr)) == -1){
        perror("bind failed\n");
        exit(-1);
    }
    //Listening
    if(listen(sock_listen,10) == -1){
        perror("listeng failed\n");
        exit(-1);
    }
    return sock_listen;
}
/*
    start the socket
*/
int StartSocket(int sock_listen,struct sockaddr_in *sock_client){
    socklen_t socketLen = sizeof(*sock_client);
    //调用 accept 函数接收一个客户端连接请求（队头请求）
    //如果成功，返回值为一个套接字文件描述符，这个套接字是和该客户端一一对应的
    //这个套接字专门用于和对应的客户端通信，所以通常称它为连接套接字。
    int sock_conn = accept(sock_listen, (struct sockaddr*)&(*sock_client), &socketLen); //第二个为结构体的指针，第三个参数为结构体的长度
    if(sock_conn == -1){
        perror("accept\n");
        exit(-1);
    }
    return sock_conn;
}
/*
    Connect with Server
*/
void SocketClientInit(struct sockaddr_in *myaddr){
    (*myaddr).sin_family = AF_INET;
    (*myaddr).sin_addr.s_addr = inet_addr("127.0.0.1");
    (*myaddr).sin_port = htons(9734);
}
int StartClientConnect(int sock_client,struct sockaddr_in *myaddr){
    if(connect(sock_client,(struct sockaddr*)myaddr,sizeof(*myaddr)) == -1){
        perror("connect error\n");
        exit(-1);
    }
    printf("Connect successfully\n");
    return 1;
}