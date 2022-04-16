#ifndef _MENU_H
#define _MENU_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void MenuForManager(){
    printf("****************The InterMusic***************\n");
    printf("---------------------------------------------\n");
    printf("**************(1) View all songs ************\n");
    printf("**************(2) Add new song  ************\n");
    printf("**************(3) Listening songs  ************\n");
    printf("**************(4) Delete song******************\n");
    printf("**************(5) Add new customer*************\n");
    printf("**************(6) Del customer*************\n");
    printf("**************(7) Find songs*************\n");
    printf("Please input your choose : ");
}
void MenuForNormal(){
    printf("****************The InterMusic***************\n");
    printf("---------------------------------------------\n");
    printf("**************(1) Listening songs ************\n");
}
#endif