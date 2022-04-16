#include "head.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int stu_num =  MIN;
int room_num = MIN_0;
int randNum[MAX] = {0};
int roomAllStorage = MIN_0;
int ans[MAX][MAX];

void read() {
    FILE * fp;
    fp = fopen("students.txt", "r++");
    while(!feof(fp)) {
        fscanf(fp, "%s%s%s%s", stu[stu_num].number, stu[stu_num].name, stu[stu_num].cla, stu[stu_num].major);
        stu[stu_num].ans = stu_num;
        stu_num++;
    }
    stu_num--;
    printf("stu_num = %d\n", stu_num);
    //print(stu, stu_num);
    FILE * fs;
    fs = fopen("rooms.txt", "r++");
    while(!feof(fs)) {
        fscanf(fs, "%s %d", room[room_num].pos, & room[room_num].storage);
        roomAllStorage += room[room_num].storage;
        room_num++;
    }
    room_num--;
    fclose(fp);
    fclose(fs);
}
void bubble_sort(int arr[], int len) {
        int i, j, temp;
        for(i = 0; i < len - 1; i++)
            for(j = 0; j < len - 1 - i; j++)
                if(arr[j] > arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
    }
    // rand create a serial numbers between 1 and stu_num without repeat
void Rand() {
    srand((unsigned) time(0));
    int a = 0, i = 0;
    int flag = 0;
    while(1 && i <= stu_num - 1) {
        a = rand() % stu_num + 1;
        for(int j = 0; j < i; j++) {
            if(randNum[j] == a) {
                flag = 1;
            }
        }
        if(flag != 1) {
            randNum[i++] = a - 1;
        } else {
            flag = 0;
        }
    }
    // //bubble_sort( & randNum, i);
    // for(int m = 0; m < i; m++) {
    //     printf("%d %d\n", m, randNum[m]);
    // }
}
int solve() {
    if(roomAllStorage < stu_num) {
        return -1;
    } else {
        int cout = 0;
        int leftSer[1000] = {
            0
        };
        int left_num = 0;
        int acc_i, acc_j;
        for(int i = 0; i < room_num; i++) {
            for(int j = 0; j < room[i].storage; j++) {
                if(cout >= stu_num) {
                    break;
                }
                int fl = 0;
                int ct = randNum[cout];
                if(j == 0) {
                    ans[i][j] = ct;
                    cout++;
                } else {
                    if((strcmp(stu[ct].major, stu[ans[i][j - 1]].major) != 0) && (strcmp(stu[ct].cla, stu[ans[i][j - 1]].cla) != 0)) {
                        ans[i][j] = ct;
                        cout++;
                        fl = 1;
                    } else {
                        leftSer[left_num++] = ct;
                        cout++;
                    }
                }
                int flag = 0;
                if(ans[i][j] == -1) {
                    if(left_num != 0) {
                        for(int k = 0; k < left_num; k++) {
                            int le_ser = leftSer[k];
                            if((strcmp(stu[le_ser].major, stu[ans[i][j - 1]].major) != 0) && (strcmp(stu[le_ser].cla, stu[ans[i][j - 1]].cla) != 0)) {
                                ans[i][j] = le_ser;
                                flag = 1;
                                if(k == left_num - 1) {
                                    leftSer[k] == -1;
                                    left_num--;
                                } else {
                                    for(int c = k + 1; c < left_num; c++) {
                                        leftSer[c - 1] = leftSer[c];
                                    }
                                    leftSer[left_num - 1] = -1;
                                    left_num--;
                                }
                                break;
                            }
                        }
                        //meaning without break,so...;
                        if(flag == 0) {
                            j--;
                        }
                    } else {
                        j--;
                    }
                }
            }
        }
        for(int i = 0; i < room_num; i++) {
            for(int j = 0; j < room[i].storage; j++) {
                if(ans[i][j] == -1 && j == 0) {
                    for(int f = 0; f < left_num; f++) {
                        if((leftSer[f] != -1) && (strcmp(stu[leftSer[f]].cla, stu[ans[i][j + 1]].cla) != 0)) {
                            ans[i][j] = leftSer[f];
                            leftSer[f] = -1;
                            break;
                        }
                    }
                } else if(ans[i][j] == -1 && j == room[i].storage - 1) {
                    for(int f = 0; f < left_num; f++) {
                        if((leftSer[f] != -1) && (strcmp(stu[leftSer[f]].cla, stu[ans[i][j - 1]].cla) != 0)) {
                            ans[i][j] = leftSer[left_num];
                            leftSer[left_num] = -1;
                            break;
                        }
                    }
                } else if(ans[i][j] == -1) {
                    for(int f = 0; f < left_num; f++) {
                        if((leftSer[f] != -1) && (strcmp(stu[leftSer[f]].cla, stu[ans[i][j - 1]].cla) != 0) && (strcmp(stu[leftSer[f]].cla, stu[ans[i][j + 1]].cla) != 0)) {
                            ans[i][j] = leftSer[f];
                            leftSer[f] = -1;
                            break;
                        }
                    }
                }
            }
        }
        
    }
}
void print(FILE *fout){
    fout = fopen("out.csv", "w++");
    
    for(int i = 0; i < room_num; i++) {
        fprintf(fout,"%s : \n",room[i].pos);
        for(int j = 0; j < room[i].storage; j++) {
            if(ans[i][j] != -1) {
                fprintf(fout, "%s   %s    %s    %s    %d\n", stu[ans[i][j]].number, stu[ans[i][j]].name, stu[ans[i][j]].major,
                                        stu[ans[i][j]].cla,j+1);
            }
        }
        fprintf(fout,"\n");
    }
    fclose(fout);
}