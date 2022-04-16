#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#define MAX_LEN 1024
struct Message{
    char kind[20];
    char buff[1024];
    int len;
};

#endif