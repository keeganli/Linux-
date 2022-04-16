#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include "SocketInit/sockinit.h"
#include "SocketVari/sockvari.h"
#include "DataBaseContro/login.h"
#include "DataBase/message.h"
#include "DataBaseContro/musicContro.h"
char tmpbuff[MAX_LEN];
struct User_Info user_info;
int StartPthreadConn(struct s_info ts,int sock_cnn,struct sockaddr_in *sock_client){
    if(sock_cnn == -1){
        perror("sock_cnn failed\n");
    }
    ts.conn = sock_cnn;
    ts.cliaddr = *sock_client;
}
void solve1(int ts_cnno,struct Message message){
    
        if(strcmp(message.kind,"1") == 0){
            int len = 0;
            int row,col;
            char **result = SelectAllMusicInfo(&len,&row,&col,ts_cnno);
            printf("Sum : %d\n",len);
            //struct Message_Music_Info message_music_info;
            //memcpy(&message_music_info.len,&len,sizeof(len));
            //message_music_info.music_info = (struct Music_Info*)malloc(sizeof(struct Music_Info)*100);
            //memcpy(message_music_info.music_info,musicList,sizeof(*musicList));
            //printf("%d\n",message_music_info.len);
        }else if(strcmp(message.kind,"3") == 0){
            char tmpS[100][MAX_LEN];
            char tmpSS[100][MAX_LEN];
            //printf("Message.len = %d\n",message.len);
            int slen = atoi(message.buff);
            for(int i = 0;i < slen;i++){
                recv(ts_cnno,tmpS[i],MAX_LEN,0);
                strcpy(tmpSS[i],"/home/lkq123/Desktop/pro/work1/EXP8-MusicAp/resource/");
                strcat(tmpSS[i],tmpS[i]);
            }
            printf("%d\n",slen); 
            //char tmpS[MAX_LEN] = "/home/lkq123/Desktop/pro/work1/EXP8-MusicAp/resource/";
            //strcat(tmpS,message.buff);
            for(int i = 0;i < slen;i++){
                printf("%s\n",tmpSS[i]);
                FILE *fd = fopen(tmpSS[i],"rb");
                char revbuff[MAX_LEN];
                int k;
                while(!feof(fd)){
                    fread(revbuff,1,MAX_LEN,fd);
                    if((k = send(ts_cnno,revbuff,MAX_LEN,0)) <= 0){
                        break;
                    }
                    memset(revbuff,'\0',MAX_LEN+1);
                }
                memset(revbuff,'\0',MAX_LEN+1);
                send(ts_cnno,"END",MAX_LEN,0);
                fclose(fd);  
            }
                  
        }else if(strcmp(message.kind,"4") == 0){
            printf("%s\n",message.buff); 
            char tmpS[MAX_LEN] = "rm /home/lkq123/Desktop/pro/work1/EXP8-MusicAp/resource/";
            strcat(tmpS,message.buff);
            system(tmpS);
        }else if(strcmp(message.kind,"5") == 0){
            char username[20];
            char password[20];
            char userkind[20];
            recv(ts_cnno,username,20,0);
            recv(ts_cnno,password,20,0);
            recv(ts_cnno,userkind,20,0);    
            regUser(username,password,userkind);
        }else if(strcmp(message.kind,"6") == 0){
            char username[20];
            recv(ts_cnno,username,20,0);
            delUser(username);
        }else if(strcmp(message.kind,"7") == 0){
            char keyWord[20];
            recv(ts_cnno,keyWord,20,0);
            char **res = selectByKeyword(ts_cnno,keyWord);
        }
        //sleep(5);
}
void solve2(int ts_conn,struct Message message){

}
void *do_work(void *arg){
    int n,i;
    struct s_info *ts = (struct s_info*)arg;
    char buff[MAX_LEN];
    char str[INET_ADDRSTRLEN];
    pthread_detach(pthread_self());//在创建线程前设置线程创建属性,设为分离态,效率高

    //check user
    struct User_Info user_info;
    recv(ts->conn,buff,sizeof(buff),0);
    memcpy(&user_info,buff,sizeof(buff));
    printf("username : %s\npassword : %s\n",user_info.username,user_info.password);
    //
    struct User_Info user_infoBack;
    user_infoBack = loginCheck(user_info.username,user_info.password);
    //printf("%s\n",filename);
    if(strcmp(user_infoBack.username,user_info.username) == 0){
        printf("%s %s %s\n",user_infoBack.username,user_infoBack.password,user_infoBack.level);
        send(ts->conn,(char*)&user_infoBack,sizeof(user_infoBack),0);
    }else {
        strcpy(user_infoBack.username,"ERROR");
        send(ts->conn,(char*)&user_infoBack,sizeof(user_infoBack),0);
    }
    while(1){
        printf("Waiting...\n");
        struct Message message;
        int flag = 0;
        char ordBuff[MAX_LEN];
        
        recv(ts->conn,&message,MAX_LEN,MSG_WAITALL);
        
       // memcpy(&message,ordBuff,sizeof(ordBuff));
        printf("choose = %s\n",message.kind);
        if(strcmp(message.kind,"2") !=0){
           solve1(ts->conn,message); 
        }else {
            flag = 1;
            //printf("%s\n",message.buff);
            char loc[100] = "/home/lkq123/Desktop/pro/work1/EXP8-MusicAp/resource/";
            addSong(message.buff);
            strcat(loc,message.buff);
            printf("%s\n",loc);
            FILE * fp = fopen(loc,"wb");
            char revbuff[MAX_LEN];
            int k;
            while((k = recv(ts->conn,revbuff,sizeof(revbuff),0)) > 0){
            //printf("%s\n",revbuff);
                if(strcmp(revbuff,"END") == 0){
                    break;
                }
                fwrite(revbuff,sizeof(char),k,fp);
            }
            fclose(fp);
            
        }
    }
    close(ts->conn);
   // exit(0);
}
int main()
{
    //init socket
    sock_server = SocketServerInit(&myaddr,ADDRESS,PORT);

    //start socket and accept Waiting...
    while(1){
        ///int sock_conn = StartSocket(sock_server,&sock_client);//start waiting for request from client
        socklen_t socketLen = sizeof(sock_client);
        //调用 accept 函数接收一个客户端连接请求（队头请求）
        //如果成功，返回值为一个套接字文件描述符，这个套接字是和该客户端一一对应的
        //这个套接字专门用于和对应的客户端通信，所以通常称它为连接套接字。
        int sock_conn = accept(sock_server, (struct sockaddr*)&sock_client, &socketLen); //第二个为结构体的指针，第三个参数为结构体的长度
        if(sock_conn == -1){
            perror("accept\n");
            exit(-1);
        }
        //when get a connection,start a new thread 
        /*
            通过accept函数与客户端socket进行三次握手连接创建的通信描述符与客户端进行数据传输。
            此时可以将accept函数的连接设置为多线程形式，轮训监听，
            每获取到一个客户端的连接，则创建一个子线程专门用于和该客户端进行通信。
        */
        //int status = StartPthreadConn(ts[pid_cnt], sock_conn,&sock_client);
        if(sock_conn == -1){
            perror("sock_cnn failed\n");
        }
        ts[pid_cnt].conn = sock_conn;
        ts[pid_cnt].cliaddr = sock_client;
        pthread_create(&pid_client,NULL,do_work,(void*)&ts[pid_cnt]);
        pid_cnt++;
    }
    return 0;
}