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
    struct Student *next;
}*stu = NULL;
int stu_num = 0;
void *thread_result;
static void cleanup_handler(void *arg){
    free(arg);
    (void)pthread_mutex_unlock(&mtx);
}
void *thread_Read(void *arg){
    struct Student *p;
    printf("Reading.....\n");
    
    //stu = (struct Student *)malloc(sizeof(struct Student));
    FILE * fp;
    fp = fopen("students.txt", "r++");
    while(!feof(fp)) {
        p = (struct Student*)malloc(sizeof(struct Student));
        fscanf(fp, "%s%s%s%s", p->no, p->name, p->age, p->sex);
        pthread_mutex_lock(&mtx);
        p->next = stu;
        stu = p;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
    }
    fclose(fp);

}
void *thread_Write(void *arg){
    printf("Writing...\n");
    struct Student *p = (struct Student*)malloc(sizeof(struct Student));
    p->next = NULL;
    //p = (struct Student *)malloc(sizeof(struct Student)*MAX_SIZE);
    //pthread_cleanup_push(cleanup_handler,p);
    while(1){
        pthread_mutex_lock(&mtx);
        while(stu == NULL){
            pthread_cond_wait(&cond,&mtx);
            struct Student *ct = p->next;
            if(p->next == NULL){
                p->next = stu;
            }else {
                while(ct != NULL){
                    if(stu->no < ct->no){
                        //qian cha
                        stu->next = ct->next;
                        ct->next = stu;
                        char tmpName[20],tmpNo[20],tmpSex[20],tmpAge[20];
                        strcpy(tmpNo,ct->no);strcpy(tmpName,ct->name);strcpy(tmpSex,ct->sex);strcpy(tmpAge,ct->age);
                        strcpy(ct->no,stu->no);strcpy(ct->name,stu->name);strcpy(ct->age,stu->age);strcpy(ct->sex,stu->sex);
                        strcpy(stu->no,tmpNo);strcpy(stu->name,tmpName);strcpy(stu->age,tmpAge);strcpy(stu->sex,tmpSex);
                    }
                    ct = ct->next;
                }
            }
           
            stu = stu->next;
        }
        pthread_mutex_unlock(&mtx);
        
        //return ;
    }
    //pthread_cleanup_pop(0);
    FILE *fout;
    fout = fopen("students.txt","w++");
    struct Student *as = p->next;
    while(as != NULL){
        fprintf(fout, "%s %s %s %s\n", as->no,as->name, as->age,as->sex);
        as = as->next;
    }            
    fclose(fout);
}
int main()
{
    int res_read,res_write;
    pthread_t thread_read,thread_write;

    //create Thread Read
    res_read = pthread_create(&thread_read,NULL,thread_Read,NULL);
    if(res_read!=0){
        perror("Creat Thread Read Error\n");
        exit(EXIT_FAILURE);
    }
    printf("Create Thread read Successfully\n");

    //create Thread Write 
    res_write = pthread_create(&thread_write,NULL,thread_Write,NULL);
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