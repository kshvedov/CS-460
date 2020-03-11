/***************************************************************/
//Konstantin Shvedov
/****************************L2U.C******************************/
// transforms text from lower to upper
//can either fo file to file or stdin to stdout
/***************************************************************/
#include "ucode.c"
#include "xtr.c"

int main(int argc, char *argv[ ])
{
    char c;
    int fd = 0, rd = 0;
    if(argc == 3) // if from file to file
    {
        close(0);
        fd = open(argv[1], O_RDONLY);
        close(1);
        open(argv[2], O_WRONLY|O_CREAT);
    }
    if(argc == 2 || argc > 3)
        exit(0);

    rd = redirection(); //used to make sure that we are piping
    
    while(read(0, &c, 1) > 0)
    {
        char cr = '\r';
        if(fd == 0 && c == '\r' && rd == 0)
            c = '\n';
        if (c >= 'a' && c <= 'z')
            c = c - ('a' - 'A');
        write(1, &c, 1);
        if(fd == 0 && c == '\n')
            write(1, &cr, 1);
    }
}