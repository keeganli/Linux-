#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_SIZE 128
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct Student{
    char name[20];
    char no[20];
    char sex[20];
    char age[20];
}*stu = NULL;
int stu_num = 0;
static void cleanup_handler(void *arg){
    free(arg);
    (void)pthread_mutex_unlock(&mtx);
}
void Sort(){
    for(int i = 0;i < stu_num;i++){
        for(int j = 0;j < stu_num-i-1;j++){
            if(strcmp(stu[j].no,stu[j+1].no) > 0){
                struct Student tmpStu;
                tmpStu = stu[j];
                stu[j] = stu[j+1];
                stu[j+1] = tmpStu;
            }
        }
    }
}
void *thread_Read(void *arg){
    printf("Reading.....\n");
    pthread_mutex_lock(&mtx);
    stu = (struct Student *)malloc(sizeof(struct Student)*MAX_SIZE);
    FILE * fp;
    fp = fopen("students.txt", "r++");
    while(!feof(fp)) {
        fscanf(fp, "%s%s%s%s", stu[stu_num].no, stu[stu_num].name, stu[stu_num].age, stu[stu_num].sex);
        //printf("%s %s %s %s\n", stuS[stu_num].no, stuS[stu_num].name, stuS[stu_num].age, stuS[stu_num].sex);
        stu_num++;
    }
    fclose(fp);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);

}

void *thread_Write(void *arg){
    printf("Writing...\n");
        
    pthread_cond_wait(&cond,&mtx);
    printf("1\n");
    Sort();
    FILE *fout;
    fout = fopen("ans.txt","w++");
    for(int i = 0;i < stu_num;i++){
        fprintf(fout, "%s %s %s %s\n", stu[i].no, stu[i].name, stu[i].age,stu[i].sex);
    }
    fclose(fout);
        
    pthread_mutex_unlock(&mtx);
}
char message[] = "hello";
void *thread_result;

int main()
{
    int res_read,res_write;
    pthread_t thread_read,thread_write;

    //create Thread Read
    res_read = pthread_create(&thread_read,NULL,thread_Read,(void*)message);
    if(res_read!=0){
        perror("Creat Thread Read Error\n");
        exit(EXIT_FAILURE);
    }
    printf("Create Thread read Successfully\n");

    //create Thread Write 
    res_write = pthread_create(&thread_write,NULL,thread_Write,(void*)message);
    if(res_write!=0){
        perror("Create thread Write Error\n");
        exit(EXIT_FAILURE);
    }
    printf("Create Thread Write Successfully\n\n");

    //Waiting for Thread Read Finished
    printf("waiting for thread_read finish\n");
    int res = pthread_join(thread_read,&thread_result);
    if(res!=0){
        perror("Thread Read Join Error\n");
    }
    printf("Thread Read Join\n");
    exit(EXIT_SUCCESS);
    pthread_cancel(thread_Read);
    //Waiting for Thread Write Finished
    printf("waiting for thread_write finish\n");
    res = pthread_join(thread_write,&thread_result);
    if(res!=0){
        perror("Thread Write Join Error\n");
    }
    printf("Thread Write Join\n");
    exit(EXIT_SUCCESS);
    pthread_exit(thread_Read);
    pthread_exit(thread_Write);
}