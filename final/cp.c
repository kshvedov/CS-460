/***************************************************************/
//Konstantin Shvedov
/*****************************CP.C******************************/
// Copies file from one location to another, uses cat
/***************************************************************/
#include "ucode.c"

int main(int argc, char *argv[ ])
{
    if (argc != 3)
        exit(0);
    char cmd[256];
    strcpy(cmd, "cat ");
    strcat(cmd, argv[1]);
    close(1);
    open(argv[2], O_WRONLY|O_CREAT);
    exec(cmd);
}