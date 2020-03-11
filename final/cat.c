/***************************************************************/
//Konstantin Shvedov
/****************************CAT.C******************************/
// My cat, this cat can print from anywhere to anywhere, no matter
// the redirections and amount of pipes
/***************************************************************/

#include "ucode.c"
#include "xtr.c"

#define BLKSIZE 4096

// main for cat
int main(int argc, char *argv[ ])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>KAOS CAT ROAR<<<<<<<<<<<<<<<<<<<<<<\n\r");
    int fd, afd, in = 0, out, i, m, n, len, change;
    char buf[BLKSIZE] = "", c = 0, mytty;
    STAT fs;
    fd = 0; // default to stdin
    afd = 0;
    if (argc > 1){
        close(0);
        fd = open(argv[1], O_RDONLY);
        in = 1;
        if (fd < 0) exit(1); // file couldnt be open, quits
    }

    //printi(fd);
    // checks if the file has been redirected
    afd = redirection();
    out = isfile(1);

    i = 0;
    while (n = read(fd, &c, 1)){
        if (in == 0) //if reading from stdin
        {
            if (afd == 0) //if more than three cats
            {
                if (c == 4) //ctrl-d is pressed
                {
                    exit(0);
                }
                else if (c == '\r')// enter is hit, buffer is printed
                {
                    c = '\n', cr = '\r';

                    buf[i++] = '\n';
                    if (out == 1)
                    {
                        write(2, &c, 1);
                        write(2, &cr, 1);
                    }
                    else
                    {
                        write(1, &c, 1);
                        write(1, &cr, 1);
                        buf[i++] = '\r';
                    }

                    buf[i] = '\0';
                    len = (int)strlen(buf);
                    write(1, buf, len);
                    i = 0;
                }
                else // while return isnt pressed
                {   
                    cr = '\r';
                    buf[i++] = c;
                    if (out == 1)
                    {
                        write(2, &c, 1);
                    }
                    else
                    {
                        write(1, &c, 1);
                        if (c == '\n')
                            write(1, &cr, 1);
                    }
                }
            }
            else
            {
                char cr = '\r';
                // no matter if there is an '\r' made
                // new one printed after '\n'
                if (c == '\n' && out == 0)
                {
                    write(1, &c, 1);
                    write(2, &cr, 1);
                }
                else if (c == '\r' && out == 0) //doesnt pass on '\r' values
                    continue;
                else                //all other characters are written
                    write(1, &c, 1);
            }
        }
        else
        {
            if(out == 0) // if outputing to stdout
            {
                mputc(c);
            }
            else
            {
                write(1, &c, 1);
            }
        }
    }
}