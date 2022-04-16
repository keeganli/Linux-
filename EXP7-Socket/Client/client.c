#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "head.h"
#include <stdlib.h>
int main()
{
    
    while(1){
        int sock_client = socket(AF_INET,SOCK_STREAM,0);
        if(sock_client == -1){
            perror("socket error\n");
            return 1;
        }
        struct sockaddr_in myaddr;
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        myaddr.sin_port = htons(9734);
        if(connect(sock_client,(struct sockaddr*)&myaddr,sizeof(myaddr)) == -1){
            perror("connect error\n");
            return 1;
        }
        printf("Connect successfully\n");
        menu();
        int choose;
        scanf("%d",&choose);
        char msgSend[20];
        if(choose == 1){
            msgSend[0] = '1';
            send(sock_client,msgSend,sizeof(msgSend),0);
            FILE * fp = fopen("recv.txt","wb+");
            char revbuff[MAX_LEN];
            int k;
            while((k = recv(sock_client,revbuff,sizeof(revbuff),0)) > 0){
                printf("%s\n",revbuff);
                fwrite(revbuff,sizeof(char),k,fp);
            }
            fclose(fp);
            //system("cat recv.txt");
        }else if(choose == 2){
            msgSend[0] = '2';
            send(sock_client,msgSend,sizeof(msgSend),0);
            printf("Please input the filename : ");
            char filename[100];
            scanf("%s",filename);
            send(sock_client,filename,sizeof(filename),0);
            char msgRecv[2];
            recv(sock_client,msgRecv,sizeof(msgRecv),0);
            if(msgRecv[0] == 's'){
                printf("Delete Successfully\n");
            }else {
                printf("Delete Failed\n");
            }
        }else if(choose == 3){
            msgSend[0] = '3';
            send(sock_client,msgSend,sizeof(msgSend),0);
            printf("Please input the keyWord's num and keyworld: ");
            int key_num = 0;
            scanf("%d",&key_num);
            char keyWord[key_num][100];
            for(int i = 0;i < key_num;i++){
                scanf("%s",keyWord[i]);
            }
            //key de ge shu
            send(sock_client,&key_num,sizeof(key_num),0);
            //key
            send(sock_client,keyWord,sizeof(keyWord),0);
            FILE * fp = fopen("ans.txt","wb+");
            char revbuff[MAX_LEN];
            int k;
            while((k = recv(sock_client,revbuff,sizeof(revbuff),0)) > 0){
                printf("%s\n",revbuff);
                fwrite(revbuff,sizeof(char),k,fp);
            }
            fclose(fp);
        }else if(choose == 4){
            msgSend[0] = '4';
            send(sock_client,msgSend,sizeof(msgSend),0);
            printf("Please input the file'name and content : ");
            char filename[20];
            scanf("%s",filename);
            char content[MAX_LEN];
            scanf("%s",content);
            send(sock_client,filename,sizeof(filename),0);
            send(sock_client,content,MAX_LEN,0);

        }
        close(sock_client);
        //menu();
        
    }
    
    
}
