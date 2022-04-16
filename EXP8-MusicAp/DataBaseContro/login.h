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
#define DATE_BASE "DataBase/User_Info.db"
#define SELECT_BY_USERNAME "select * from User where username = "
#define INSERT "insert into User values("
#define DELETE "delete from User where username = "
#define AND " and "
#define INSERT_MUSIC "insert into Music_Info values("
#define USERNAME " username = "
#define PASSWORD " password = "
struct User_Info{
    char username[20];
    char password[20];
    char level[20];
};
void printResult(char **result,int row,int col){
    //printf("Username\tPassword\t\n");
    for(int i = 0;i < row+1;i++){
        for(int j = 0;j < col;j++){
            printf("%s\t",result[j+i*col]);
        }
        printf("\n");
    }
}
void freeSql(char *errorMeg,char **result,sqlite3 *db){
    sqlite3_free(errorMeg);
    sqlite3_free_table(result);
    sqlite3_close(db);
}
struct User_Info loginCheck(char username[20],char password[20]){
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    int row = 0,col = 0;
    char sql[100] = SELECT_BY_USERNAME;
        strcat(sql,"'");strcat(sql,username);strcat(sql,"'");
        strcat(sql,AND);
        strcat(sql,PASSWORD);strcat(sql,"'");strcat(sql,password);strcat(sql,"'");
    printf("%s\n",sql);

    rc = sqlite3_get_table(db,sql,&result,&row,&col,&errorMeg);
    printResult(result,row,col);
    struct User_Info user_info;
    if(row == 1){
        printf("Login Succeed\n");
        strcpy(user_info.username,result[(row)*col]);
        strcpy(user_info.password,result[1+(row)*col]);
        strcpy(user_info.level,result[2+(row)*col]);
        freeSql(errorMeg,result,db);
        return user_info;
    }else{
        freeSql(errorMeg,result,db);
        return user_info;
    } 
    //return user_info;
}
void regUser(char username[20],char password[20],char level[20]){
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    int row = 0,col = 0;
    char sql[100] = INSERT;
        strcat(sql,"'");
        strcat(sql,username);
        strcat(sql,"','");
        strcat(sql,password);
        strcat(sql,"','");
        strcat(sql,level);
        strcat(sql,"')");
    printf("%s\n",sql);
    sqlite3_exec(db,sql,0,0,&errorMeg);

}
void delUser(char username[20]){
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    int row = 0,col = 0;
    char sql[100] = DELETE;
        strcat(sql,"'");
        strcat(sql,username);
        strcat(sql,"'");
    printf("%s\n",sql);
    sqlite3_exec(db,sql,0,0,&errorMeg);
    
}
void addSong(char musicname[MAX_LEN]){
    char musicName[20];
    strcpy(musicName,musicname);
    sqlite3 *db = NULL;
    int rc = sqlite3_open(DATE_BASE,&db);
    char **result;
    char *errorMeg;
    int row = 0,col = 0;
    char sql[100] = INSERT_MUSIC;
        strcat(sql,"'");
        strcat(sql,musicName);
        strcat(sql,"'");
        strcat(sql,",");
        strcat(sql,"'");
        strcat(sql,"NULL");
        strcat(sql,"'");
        strcat(sql,",");
        strcat(sql,"'");
        strcat(sql,"NULL");
        strcat(sql,"')");
    printf("%s\n",sql);
    sqlite3_exec(db,sql,0,0,&errorMeg);
}