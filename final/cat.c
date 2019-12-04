#include "ucode.c"

#define BLKSIZE 1024


int main(int argc, char *argv[ ])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>>>KAOS CAT<<<<<<<<<<<<<<<<<<<<<<<<<\n\r");
    int fd, i, m, n, len;
    char buf[4096] = "", c = 0;
    fd = 0; // default to stdin

    if (argc > 1){
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) exit(1);
    }

    i = 0;
    while (n = read(fd, &c, 1)){
        //m = write(1, buf, 3);
        if (fd == 0)
        {
            if (c == '\r')
            {
                mputc('\n');
                buf[i++] = '\n';
                buf[i++] = '\r';
                buf[i] = '\0';
                len = (int)strlen(buf);
                write(1, buf, len);
                i = 0;
            }
            else
            {
                buf[i++] = c;
                mputc(c);
            }
            
        }
        else
        {
            mputc(c);
        }
    }
}