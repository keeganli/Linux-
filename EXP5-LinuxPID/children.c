#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <fcntl.h>
#include "head.h"
int write_fd,read_fd;
struct msg msg1,msg2;
int stu_num = 0;            
struct Student tmpStu;
struct Student
{
    char name[20];
    char no[20];
    char sex[20];
    char age[20];
}stuB[128],stuS[128];

int Standread() {
    stu_num = 0;
    FILE * fp;
    fp = fopen("students.txt", "r++");
    while(!feof(fp)) {
        fscanf(fp, "%s%s%s%s", stuS[stu_num].no, stuS[stu_num].name, stuS[stu_num].age, stuS[stu_num].sex);
        //printf("%s %s %s %s\n", stuS[stu_num].no, stuS[stu_num].name, stuS[stu_num].age, stuS[stu_num].sex);
        stu_num++;
    }
    fclose(fp);
    return stu_num;
}
int DeleteStu(char name[20]){
    system("rm -f students.txt");
    system("touch students.txt");
    FILE *fout;
    fout = fopen("students.txt","w++");
    for(int i = 0;i < stu_num;i++){
        if(strcmp(stuS[i].name,name) != 0){
            fprintf(fout, "%s %s %s %s\n", stuS[i].no, stuS[i].name, stuS[i].age,stuS[i].sex);
        }
    }
    fclose(fout);
    return 1;
}
int AddStu(){
    strcpy(stuS[stu_num].no ,tmpStu.no);
    strcpy(stuS[stu_num].name,tmpStu.name);
    strcpy(stuS[stu_num].age,tmpStu.age);
    strcpy(stuS[stu_num].sex,tmpStu.sex);
    printf("%s %s %s %s\n", stuS[stu_num].no, stuS[stu_num].name, stuS[stu_num].age,stuS[stu_num].sex);
    stu_num++;
    FILE *fout;
    fout = fopen("students.txt","w++");
    for(int i = 0;i < stu_num;i++){
        fprintf(fout, "%s %s %s %s\n", stuS[i].no, stuS[i].name, stuS[i].age,stuS[i].sex);
    }
    //fprintf(fout,"%s %s %s %s\n", tmpStu.no,tmpStu.name,tmpStu.age,tmpStu.sex);
    fclose(fout);
    return 1;
}
int main()
{
    init(SEND_FIFO);
    init(RECEIVE_FIFO);
    
    read_fd = open(SEND_FIFO,O_RDONLY);
    write_fd = open(RECEIVE_FIFO,O_WRONLY);
    
    while(1){
        read(read_fd,&msg1,sizeof(msg1));
        stu_num = Standread();
        if(msg1.ms_kind == 1){
            write(write_fd,stuB,sizeof(stuB));
            printf("Select %s Successfully\n",msg1.mes);
        }else if(msg1.ms_kind == 2){
            int re = DeleteStu(msg1.mes);
            if(re == 1){
                msg2 = msg1;
                write(write_fd,&msg2,sizeof(msg2));
                printf("Delete %s successful!\n",msg1.mes);
            }
        }else if(msg1.ms_kind == 3){
            
            read(read_fd,&tmpStu,sizeof(tmpStu));
            int re = AddStu();
            if(re == 1){
                msg2 = msg1;
                write(write_fd,&msg2,sizeof(msg2));
                printf("Add Successfully! %s\n",tmpStu.name);
            }
        }else if(msg1.ms_kind == 4){
            write(write_fd,stuB,sizeof(stuB));
            printf("Show All Students Successfully\n");
        }
    }
    
    
    
}