struct msg{
    int kind;
    char mess[20];
};
#define MAX_LEN 2048
#define SOCKET_ERROR -1
void menu(){
    printf("       Welcome to the System of fileManager\n");
    printf("------------------------------------------------------\n");
    printf("     Please Choose the way which you like:\n");
    printf("        1.     show all files on the server\n                \n");
    printf("        2.delete the file with the specified filename\n             \n");
    printf("        3.   Find files with specified content\n                     \n");
    printf("        4.          add new file\n                     \n");
    printf("        5.              exit\n                     \n");
    printf("-------------------------------------------------------\n");
    printf("Please Input Your Choose : ");
}