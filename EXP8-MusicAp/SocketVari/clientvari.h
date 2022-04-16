#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#define MAX_LEN 1024
int sock_client;
struct sockaddr_in myaddr;

// struct User_Info{
//     char username[20];
//     char password[20];
// };
