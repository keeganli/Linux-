#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <string.h> 
#include "head.h"
int main() {
    
    read();
    Rand();
    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            ans[i][j] = -1;
        }
    }
    int answer = solve();
    if(answer == -1){
        printf("Not enough seats!!!\n");
       
    }else{
        FILE * fout;
        print(fout); 
    }
    return 0;
}