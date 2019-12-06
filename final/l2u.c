#include "ucode.c"

int main(int argc, char *argv[ ])
{
    char c;
    if(argc == 3)
    {
        close(0);
        open(argv[1], O_RDONLY);
        close(1);
        open(argv[2], O_WRONLY|O_CREAT);
    }
    if(argc == 2 || argc > 3)
        exit(0);
    
    while(read(0, &c, 1) > 0)
    {
        if (c >= 'a' && c <= 'z')
            c = c - ('a' - 'A');
        write(1, &c, 1);
    }
}