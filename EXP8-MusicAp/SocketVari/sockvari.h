#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#define MAX_LEN 1024
#define MAX_PID_NUM 100
#define ADDRESS INADDR_ANY
#define PORT 9734
struct sockaddr_in myaddr;
struct sockaddr_in sock_client;
int sock_server;
pthread_t pid_client;
struct s_info
{
    /* data */
    struct sockaddr_in cliaddr;
    int conn;
}ts[MAX_PID_NUM];
int pid_cnt = 0;