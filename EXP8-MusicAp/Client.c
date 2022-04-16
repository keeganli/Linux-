#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sqlite3.h>
#include "SocketInit/sockinit.h"
#include "SocketVari/clientvari.h"
#include "DataBase/user.h"
#include "Util/menu.h"
#include "DataBase/message.h"
#include "DataBaseContro/musicContro.h"
char buff[MAX_LEN];
char songName[100][MAX_LEN];
char song_len[MAX_LEN];
char regname[20];
char regpassword[20];
char reguserkind[20];
char delusername[20];
char keyWord[20];
int slen = 0;
void solve(char choose[20],int len,int level){
    char loc[100] = "/home/lkq123/Desktop/pro/work1/EXP8-MusicAp/ClientRes/";
    struct Message message;
    memcpy(message.kind,choose,len);
        //printf("choose = %s\n",choose);
       // send(sock_client,(char*)&message,sizeof(message),0);
    if(level == 1){
       printf("choose = %s\n",message.kind); 
    }
    if(strcmp(choose,"1") == 0){
        //printf("1\n");
       
    }else if(strcmp(choose,"2") == 0){
        printf("Please input the song's name : ");
        char name[MAX_LEN];
        scanf("%s",name);
        strcat(loc,name);
    
        memcpy(message.buff,name,sizeof(name));
        printf("%s\n",loc);   
    }else if(strcmp(choose,"3") == 0){
        //memset(buff,'\0',sizeof(buff));
        printf("Please input the songs' name: ");
        
        scanf("%s",song_len);
        slen = atoi(song_len);
        memcpy(message.buff,song_len,sizeof(song_len));
        //memcpy(message.buff,name,MAX_LEN);
    }else if(strcmp(choose,"4") == 0){
        char buff[MAX_LEN];
        //memset(buff,'\0',sizeof(buff));
        printf("Please input the songs' name: ");
        int len;
        scanf("%d",&len);
        //memset(buff,'\0',sizeof(buff));
        //for()
        char name[MAX_LEN];
        scanf("%s",name);
        //memcpy(buff,name,sizeof(name));
        memcpy(message.buff,name,MAX_LEN);
    }else if(strcmp(choose,"5") == 0){
        printf("Please input regName : ");
        scanf("%s",regname);
        printf("Please input regPassword : ");
        scanf("%s",regpassword);
        printf("Please input regUserkind : ");
        scanf("%s",reguserkind);
    }else if(strcmp(choose,"6") == 0){
        printf("Please input the name : ");
        scanf("%s",delusername);
    }else if(strcmp(choose,"7") == 0){
        printf("Please input the keyWord : ");
        scanf("%s",keyWord);
    }
    send(sock_client,(char*)&message,MAX_LEN,0);
    if(strcmp(message.kind,"2") == 0){
        FILE *fd = fopen(loc,"rb");
        char revbuff[MAX_LEN];
        int k;
        while(!feof(fd)){
            fread(revbuff,1,MAX_LEN,fd);
            if((k = send(sock_client,revbuff,MAX_LEN,0)) <= 0){
                break;
            }
            memset(revbuff,'\0',MAX_LEN+1);
        }
        memset(revbuff,'\0',MAX_LEN+1);
        send(sock_client,"END",MAX_LEN,0);
        fclose(fd);     
    }else if(strcmp(message.kind,"3") == 0){
        char buff[MAX_LEN];
        memset(buff,'\0',sizeof(buff));
        char name[MAX_LEN];

        for(int i = 0;i < slen;i++){
            scanf("%s",name);
            strcpy(songName[i],name);
            send(sock_client,name,MAX_LEN,0);
        }
    }else if(strcmp(message.kind,"5") == 0){
        send(sock_client,regname,20,0);
        send(sock_client,regpassword,20,0);
        send(sock_client,reguserkind,20,0);
    }else if(strcmp(message.kind,"6") == 0){
        send(sock_client,delusername,20,0);
    }else if(strcmp(message.kind,"7") == 0){
        send(sock_client,keyWord,20,0);
    }
}
void RecvFromSer(char choose[20]){
    struct Message_Music_Info message_music_info;
    if(strcmp(choose,"1") == 0){
        int row,col;
        recv(sock_client,&row,sizeof(&row),0);
        recv(sock_client,&col,sizeof(&col),0);
        printf("%d %d\n",row,col);
        char buff[MAX_LEN];
        //recv(sock_client,buff,sizeof(buff),0);
        //message_music_info.music_info = (struct Music_Info*)malloc(sizeof(struct Music_Info)*200);
        //memcpy(&message_music_info,buff,sizeof(buff));
        //printf("\nIn total, there are %d songs.\n",message_music_info.len);
        // printf("Name\tAuthor\tContent\n");
        // for(int i = 0;i < message_music_info.len;i++){
        //     printf("%s\t%s\t%s\n",message_music_info.music_info[i].name,
        //             message_music_info.music_info[i].author,
        //             message_music_info.music_info[i].content);
        // }
        for(int i = 0;i < row+1;i++){
            for(int j = 0;j < col;j++){
                recv(sock_client,buff,MAX_LEN,0);
                printf("%s\t",buff);
            }
            printf("\n");
        }
    }else if(strcmp(choose,"2") == 0){
        
    }else if(strcmp(choose,"3") == 0){
        char loc[100] = "/home/lkq123/Desktop/pro/work1/EXP8-MusicAp/ClientResBack/";
        for(int i = 0;i < slen;i++){
            char tmpLoc[MAX_LEN];
            strcpy(tmpLoc,loc);
            strcat(tmpLoc,songName[i]);
            FILE * fp = fopen(tmpLoc,"wb");
            char revbuff[MAX_LEN];
            int k;
            while((k = recv(sock_client,revbuff,sizeof(revbuff),0)) > 0){
                //printf("%s\n",revbuff);
                if(strcmp(revbuff,"END") == 0){
                    break;
                }
                fwrite(revbuff,sizeof(char),k,fp);
            }
            fclose(fp);
        }
        char pla[MAX_LEN] = "mplayer /home/lkq123/Desktop/pro/work1/EXP8-MusicAp/ClientResBack/*";
        printf("Songs'list : ");
        for(int i = 0;i < slen;i++){
            printf("%s ",songName[i]);
        }
        printf("\n");
        if(fork() != -1){
            system(pla);
        }
    }else if(strcmp(choose,"7") == 0){
        char recvBUff[MAX_LEN];
        int row,col;
        recv(sock_client,&row,sizeof(&row),0);
        recv(sock_client,&col,sizeof(&col),0);
        for(int i = 0;i < row+1;i++){
            for(int j = 0;j < col;j++){
                recv(sock_client,recvBUff,MAX_LEN,0);
                printf("%s\t",recvBUff);
                memset(recvBUff,'\0',sizeof(recvBUff));
            }
            printf("\n");
        }
    }
        
}
int main()
{
    while(1){
        sock_client = CreateSocket();
        SocketClientInit(&myaddr);
        char username[20],password[20];
        struct User_Info user_info;
        if(StartClientConnect(sock_client,&myaddr) == 1){//connect successful
            printf("Please input your Username : ");
            scanf("%s",username);
            printf("Please input your Password : ");
            scanf("%s",password);
            memcpy(user_info.username,username,sizeof(username));
            memcpy(user_info.password,password,sizeof(password));
            send(sock_client,(char*)&user_info,sizeof(user_info),0);
            recv(sock_client,buff,sizeof(buff),0);
            memcpy(&user_info,buff,sizeof(buff));
            printf("\n%s,Welcome to the InterMusic\n\n",user_info.username);
            char kind[20] = "Manager";
            if(strcmp(user_info.username,"ERROR") == 0){
                printf("Login Failed,Wrong Username or Password\n");
            }else{
                if(strcmp(user_info.level,kind) == 0){
                    while(1){
                        MenuForManager();
                        char choose[20];
                        scanf("%s",choose);
                        int len = sizeof(choose);
                        solve(choose,len,1);
                        RecvFromSer(choose);

                    }
                    

                }else {
                    MenuForNormal();
                    char choose[20] = "3";
                    //scanf("%s",choose);
                    int len = sizeof(choose);
                    solve(choose,len,0);
                    RecvFromSer(choose);
                }
            }
            
            close(sock_client);
        }
    }
}