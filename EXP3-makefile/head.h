#ifndef _HEAD_H_
#define _HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#define MIN 0
#define MIN_0 0
#define MAX 10000
#define STU_MAX 100000
extern int stu_num;
extern int room_num;
#ifndef _STUDENTS_
#define _STUDENTS_
struct student
{
    char number[50];
    char name[100];
    char cla[200];
    char major[100];
    int ans;
} stu[STU_MAX];
#endif
#ifndef _ROOMS_
#define _ROOMS_
struct rooms
{
    /* data */
    char pos[10];
    int storage;
} room[MAX];
#endif
extern int randNum[MAX];
extern int roomAllStorage;
extern int ans[MAX][MAX];
void read();
void bubble_sort(int arr[], int len);
void Rand();
int solve();
void print(FILE *file);
#endif