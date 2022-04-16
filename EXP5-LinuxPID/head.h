#include <stdio.h>
#include <stdlib.h>
#define SEND_FIFO "FaCh.fifo"
#define RECEIVE_FIFO "ChFa.fifo"
#define MesgLen sizeof(struct msg) 
#define StuLen sizeof(struct Student)*128
struct msg
{
    int ms_kind;
    char mes[10];
};
void Pre(int kind);//menu

int lenBack(char a[]);//calcolate lens

void Scree();//second menu
void init(const char*fifo);



void Scree(){
    printf("Welcome to the System of Students\n");
    printf("-----------------------------------\n");
    printf("Please Choose the way which you like:\n");
    printf("          1.Basic I/O\n                \n");
    printf("          2.Standand I/O\n             \n");
    printf("          3.Quit\n                     \n");
    printf("------------------------------------\n");
    printf("Please Input Your Choose : ");
}
void init(const char* fifo) {
	if(access(fifo, F_OK) == -1) {
		if(mkfifo(fifo, 0666) < 0) {
			exit(1);
		}
	}
}
int lenBack(char a[]){
    int lenc = 0;
    for(int i = 0;i < 128;i++){
        if(a[i] != '\0'){
            lenc++;
        }else {
            return lenc;
        }
    }
}

void Pre(int kind){
    if(kind == 1){
        printf("               Basic I/O\n");
        printf("          Select Your Operate:\n");
        printf("----------------------------------------------\n");
        printf("    1.      Select Student By name\n");
        printf("    2.      Delect Student By name\n");
        printf("    3.Add New Student With Sno,Sname,Sage,Ssex\n");
        printf("    4.      Shou All Students's Info\n");
        printf("    5.              Back\n");
        printf("----------------------------------------------\n");
        printf("Please Input your Choose :  ");
    }else {
        printf("               Standand I/O\n");
        printf("          Select Your Operate:\n");
        printf("----------------------------------------------\n");
        printf("    1.      Select Student By name\n");
        printf("    2.      Delect Student By name\n");
        printf("    3.Add New Student With Sno,Sname,Sage,Ssex\n");
        printf("    4.      Shou All Students's Info\n");
        printf("    5.              Back\n");
        printf("----------------------------------------------\n");
    }
}