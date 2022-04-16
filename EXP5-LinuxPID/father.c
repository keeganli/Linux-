#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <fcntl.h>
#include <string.h>
#include "head.h"
int lenz[128] = {0};
int write_fd,read_fd;
struct msg msg1,msg2;
int stu_num = 0;

struct Student
{
    char name[20];
    char no[20];
    char sex[20];
    char age[20];
}stuB[128],stuS[128];
struct Student tmpStu;
int findStudent(char name[20],int len){
    for(int k = 0;k < len;k++){
        if(strcmp(name,stuB[k].name) == 0){
            return k;
        }
    }
    return -1;
}
void AddStu(){
    
    printf("Please Input Student's Info\n");
    printf("no   name   age   sex\n");
    scanf("%s %s %s %s",tmpStu.no,tmpStu.name,tmpStu.age,tmpStu.sex);
    msg1.ms_kind = 3;
    write(write_fd,&msg1,sizeof(msg1));
    //read(read_fd,&msg2,sizeof(msg2));
    write(write_fd,&tmpStu,sizeof(tmpStu));
    read(read_fd,&msg2,sizeof(msg2));
    printf("Add Successfully!\n");
}
void DeleteStu(int kind,char name[20]){
    msg1.ms_kind = 2;
    strcpy(msg1.mes,name);
    write(write_fd,&msg1,sizeof(msg1));
    read(read_fd,&msg2,sizeof(msg2));
    if(msg2.ms_kind == 2){
        printf("%s has been deleted successfully!\n",msg2.mes);
    }else {
        printf("Delete Failed\n");
    }
}
void divc(char buff[128][128],int len){
    for(int i = 0;i < len;i++){
        int j = 0;
        char name[20];
        char no[20];
        char sex[20];
        char age[20];
        int ct = 0;
        int a = 0,b = 0,c = 0,d = 0;
        while(buff[i][j] != '\0'){
            if(buff[i][j] == ' '){
                j++;
                ct++;
                continue;
            }
            if(ct == 0){
                no[a++] = buff[i][j];
            }else if(ct == 1){
                name[b++] = buff[i][j];
            }else if(ct == 2){
                age[c++] = buff[i][j];
            }else {
                sex[d++] = buff[i][j];
            }
            j++;
        }
        no[a] = '\0';name[b] = '\0';
        age[c] = '\0';sex[d] = '\0';
        strcpy(stuB[i].no,no);
        strcpy(stuB[i].name,name);
        strcpy(stuB[i].age,age);
        strcpy(stuB[i].sex,sex);
    }
}

int BasicRead(int fd,char buff[128][128]){
    int len = 0;
    int rl=-1;
    char c;
    
    if(fd<0){
        printf("open file error\n");
        exit(0);
    }
    int count=0;
    int l = 0;
    while((rl=read(fd,&c,1))>0){
        if(c=='\n'){
            buff[len][count]='\0';
            len++;
            lenz[l++] = count;
            count = 0;
            continue;  
        }
        buff[len][count++] = c;
    } 
    return len;
}
int Standread() {
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
void BasicIO(){
    Pre(1);
    int file;
    file = open("students.txt",O_RDWR);
    if(file == -1){
        printf("Open file failed.\n");
    }else{
        char buff[128][128];
        int num = BasicRead(file,buff);
        divc(buff,num);
        while(1){
            printf("%d\n",num);
            int choose;
            scanf("%d",&choose);
            if(choose == 1){
                printf("Please input Student's name : ");
                char name[128];
                scanf("%s",name);        
                // for(int i = 0;i < num;i++){
                //     printf("%s\n",buff[i]);
                // }
                for(int k = 0;k < num;k++){
                    if(strcmp(name,stuB[k].name) == 0){
                        printf("%s %s %s %s\n",stuB[k].no,stuB[k].name,stuB[k].age,stuB[k].sex);
                    }
                }
            }else if(choose == 2){
                char name[20];
                printf("Please Input the Student's name : ");
                scanf("%s",name);
                int ans = findStudent(name,num);
                if(ans == -1){
                    printf("Without this Student\n");
                }else {
                    system("rm -f students.txt");
                    system("touch students.txt");
                    int fd = open("students.txt",O_RDWR);
                    if(fd ==-1){
                        printf("Open Failed\n");
                    }else {
                        char tp[1];
                        tp[0] = '\n';
                        for(int w = 0;w < num;w++){
                            if(w != ans){
                                write(fd,buff[w],lenz[w]);
                                write(fd,tp,1);
                            }
                        }
                        close(fd);
                        printf("Delete Successfully\n");
                    }
                }
                //break;
            }else if(choose == 3){
                printf("%d",num);
                printf("Please Input Students' Info : \n");
                printf("no     name     age     sex\n");
                char name[20];
                char no[128];
                char sex[20];
                char age[20];
                scanf("%s%s%s%s",no,name,age,sex);
                system("rm -f students.txt");
                system("touch students.txt");
                int fd = open("students.txt",O_RDWR);
                if(fd ==-1){
                    printf("Open Failed\n");
                }else {
                    int lenn = lenBack(no) + lenBack(name) + lenBack(age) + lenBack(sex) + 3;
                    char tp[1];
                    tp[0] = '\n';
                    strcat(no," ");
                    strcat(no,name);
                    strcat(no," ");
                    strcat(no,age);
                    strcat(no," ");
                    strcat(no,sex);
                    //printf("num = %d\n",num);
                    //printf("%s\n",no);
                    no[lenn] = '\0';

                    strcpy(buff[num],no);
                    lenz[num] = lenn;
                    num++;
                    for(int w = 0;w < num;w++){
                        printf("%d   %s\n",w ,buff[w]);
                        write(fd,buff[w],lenz[w]);
                        write(fd,tp,1);
                    }
                    //write(fd,no,lenn);
                    close(fd);
                    printf("Add Successfully\n");
                }
                //break;
            }else if(choose == 4){
                for(int i = 0;i < num;i++){
                    printf("%s\n",buff[i]);
                }
            }else {
                break;
            }
            sleep(0.5);
            Pre(1);
        }
    }
    close(file);
    Menu();
}
void StandIO(){
    Pre(2);
    while(1){
        int choose;
        scanf("%d",&choose);
        if(choose == 1){
            printf("Please Input The Student's name : ");
            char name[20];
            scanf("%s",name);
            msg1.ms_kind = 1;
            strcpy(msg1.mes,name);
            write(write_fd,&msg1,MesgLen);
            read(read_fd,stuB,sizeof(stuB));
            for(int i = 0;i < stu_num;i++){
                if(strcmp(stuS[i].name,name) == 0){
                    printf("%s   %s   %s   %s\n",stuS[i].no,stuS[i].name,stuS[i].age,stuS[i].sex);
                }
            }
        }else if(choose == 2) {
            printf("Please Input The Student's name : ");
            char name[20];
            scanf("%s",name);
            DeleteStu(2,name);
            
        }else if(choose == 3){
            AddStu();
        }else if(choose == 4){
            msg1.ms_kind = 4;
            write(write_fd,&msg1,MesgLen);
            read(read_fd,stuB,sizeof(stuB));
            for(int i = 0;i < stu_num;i++){ 
                printf("%s   %s   %s   %s\n",stuS[i].no,stuS[i].name,stuS[i].age,stuS[i].sex);
            }
        }else if(choose == 5){
            break;
        }
        Pre(2);
        sleep(0.5);
    }  
    Menu();
}

void Menu(){
    Scree();
    int choose;
    scanf("%d",&choose);
    if(choose == 1){
        BasicIO();
    }else if(choose == 2){
        StandIO();
    }else {
        return ;
    }
}
int main()
{ 

    init(SEND_FIFO);
    init(RECEIVE_FIFO);
    write_fd = open(SEND_FIFO,O_WRONLY);
    read_fd = open(RECEIVE_FIFO,O_RDONLY);
    stu_num = Standread();
    Menu(); 

    return 0;
}