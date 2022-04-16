#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "head.h"
int main()
{
    int sock_listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock_listen == -1){
        perror("sock error\n");
        return 1;
    }

    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(9734);

    if(bind(sock_listen,(struct sockaddr*)&myaddr,sizeof(myaddr))){
        perror("bind failed\n");
        return 1;
    }
    //Listening
    if(listen(sock_listen,10) == -1){
        perror("listeng failed\n");
        return 1;
    }
    
    while(1){
        printf("Waiting...\n");
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        //调用 accept 函数接收一个客户端连接请求（队头请求）
        //如果成功，返回值为一个套接字文件描述符，这个套接字是和该客户端一一对应的
        //这个套接字专门用于和对应的客户端通信，所以通常称它为连接套接字。
        int sock_conn = accept(sock_listen, (struct sockaddr*)&client_addr, &len); //第二个为结构体的指针，第三个参数为结构体的长度
        if(sock_conn == -1){
            perror("accept\n");
        }
        if(fork() == 0){
            char msgrev[20];
            int ret = recv(sock_conn,msgrev,sizeof(msgrev),0);
            if(msgrev[0] == '1'){//ls
                //printf("%c\n",msgrev[0]);
                int nsend = 0;
                system("ls >> tmp.txt");
                FILE *fd = fopen("tmp.txt","r");
                char revbuff[MAX_LEN+1];
                int k;
                while(!feof(fd)){
                    fread(revbuff,1,MAX_LEN,fd);
                    if((k = send(sock_conn,revbuff,MAX_LEN,0)) <= 0){
                        break;
                    }
                    memset(revbuff,'\0',MAX_LEN+1);
                }
                fclose(fd);
                //system("rm tmp.txt");
                close(sock_conn);
                // int val = 1;
                // int ret = setsockopt(sock_conn,SOL_SOCKET,SO_REUSEADDR,(void *)&val,sizeof(int));
                // if(ret == -1)
                // {
                //     printf("setsockopt");
                //     exit(1);
                // }     
            }else if(msgrev[0] == '2'){
                char filename[100];
                int ret2 = recv(sock_conn,filename,sizeof(filename),0);
                char comd[4] = {'r','m',' ','\0'};
                strcat(comd,filename);
                int state = system(comd);
                char msgsend[2];
                    
                if(state != -1&&WIFEXITED(state)&&!WEXITSTATUS(state)){
                    msgsend[0] = 's';
                }else {
                    msgsend[0] = 'f';
                }
                send(sock_conn,msgsend,sizeof(msgsend),0);
            }else if(msgrev[0] == '3'){
                int key_num;
                int ret3_keynum = recv(sock_conn,&key_num,sizeof(key_num),0);
                char keyWord[key_num][100];
                int ret3_keyword = recv(sock_conn,keyWord,sizeof(keyWord),0);
                char grp1[10] = "grep -n \0"; 
                char grp2[10] = " -r *.txt\0"; 
                for(int i = 0;i < key_num;i++){
                    if(i == key_num - 1){
                        strcat(grp1,keyWord[i]);
                        strcat(grp1,grp2); 
                        strcat(grp1," >> keyfile.txt");
                    }else {
                        strcat(grp1,keyWord[i]);
                        strcat(grp1,grp2);
                        strcat(grp1," & ");
                    }
                }
                printf("%s\n",grp1);
                system(grp1);
                FILE *fd = fopen("keyfile.txt","rb");
                char revbuff[MAX_LEN+1];
                int k;
                while(!feof(fd)){
                    fread(revbuff,1,MAX_LEN,fd);
                    if((k = send(sock_conn,revbuff,MAX_LEN,0)) <= 0){
                        break;
                    }
                    memset(revbuff,'\0',MAX_LEN+1);
                }
                fclose(fd);
                close(sock_conn);
                //system("rm keyfile.txt");
                
            }else if(msgrev[0] == '4'){
                char filename[20];
                char content[MAX_LEN];
                int ret4_filename = recv(sock_conn,filename,sizeof(filename),0);
                int ret5_content = recv(sock_conn,content,MAX_LEN,0);
                char tch[] = "touch ";
                strcat(tch,filename);
                system(tch);
                char eho[] = "echo ";
                strcat(eho,content);
                strcat(eho," > ");
                strcat(eho,filename);
                system(eho);
            }
        }else {
            close(sock_conn);
        }
    }       
}