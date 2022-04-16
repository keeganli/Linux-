#ifndef _MUSICCONTRO_H
#define _MUSICCONTRO_H
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
#define MAX_LEN 1024
struct Music_Info{
    char name[20];
    char author[20];
    char content[30];
};
struct Message_Music_Info{
    int len;
    struct Music_Info music_info[MAX_LEN];
};
#define DATE_BASE "DataBase/User_Info.db"
#define AND " and "
void printResultM(char **result,int row,int col){
    //printf("Username\tPassword\t\n");
    for(int i = 0;i < row+1;i++){
        for(int j = 0;j < col;j++){
            printf("%s\t",result[j+i*col]);
        }
        printf("\n");
    }
}
void freeSqlM(char *errorMeg,char **result,sqlite3 *db){
    sqlite3_free(errorMeg);
    sqlite3_free_table(result);
    sqlite3_close(db);
}
char** SelectAllMusicInfo(int *len,int *r,int *w,int ts_cnno){
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    char buff[MAX_LEN];
    memset(buff,'\0',sizeof(buff));
    int row = 0,col = 0;
    char *sql = "select * from Music_Info";
    rc = sqlite3_get_table(db,sql,&result,&row,&col,&errorMeg);
    printResultM(result,row,col);
    //struct Music_Info *musicList;
    //musicList = (struct Music_Info*)malloc(sizeof(struct Music_Info)*row+1);
    //huo qu ge dan
    *len = row;
    printf("SUM : %d\n",*len);
    send(ts_cnno,&row,sizeof(&row),0);
    send(ts_cnno,&col,sizeof(&col),0);
    //send(ts_cnno,(char*)result,sizeof(result),0);
    //printf("Username\tPassword\t\n");
    for(int i = 0;i < row+1;i++){
        for(int j = 0;j < col;j++){
            printf("%s\t",result[j+i*col]);
            strcpy(buff,result[j+i*col]);
            send(ts_cnno,buff,MAX_LEN,0);
        }
        printf("\n");
    }
    //send(ts_cnno,buff,MAX_LEN,0);
    freeSqlM(errorMeg,result,db);
    return result;
}
char **selectByKeyword(int ts_cnno,char keyWord[20]){
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    char buff[MAX_LEN];
    memset(buff,'\0',sizeof(buff));
    int row = 0,col = 0;
    char sql[] = "select * from Music_Info where name like '%";
    strcat(sql,keyWord);
    strcat(sql,"%' or author like '%");
    strcat(sql,keyWord);
    strcat(sql,"%' or content like '%");
    strcat(sql,keyWord);
    strcat(sql,"%'");
    printf("%s",sql);
    rc = sqlite3_get_table(db,sql,&result,&row,&col,&errorMeg);
    send(ts_cnno,&row,sizeof(&row),0);
    send(ts_cnno,&col,sizeof(&col),0);
    //send(ts_cnno,(char*)result,sizeof(result),0);
    //printf("Username\tPassword\t\n");
    for(int i = 0;i < row+1;i++){
        for(int j = 0;j < col;j++){
            printf("%s\t",result[j+i*col]);
            strcpy(buff,result[j+i*col]);
            send(ts_cnno,buff,MAX_LEN,0);
        }
        printf("\n");
    }
    //send(ts_cnno,buff,MAX_LEN,0);
    freeSqlM(errorMeg,result,db);
    return result;
}
#endif