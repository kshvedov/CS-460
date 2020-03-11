/****************************************************************/
//Konstantin Shvedov
/****************************GREP.C******************************/
// GREP searches for a pattern from any kind of redirection
// and prints to any
/***************************************************************/

#include "ucode.c"
#include "xtr.c"

#define BLKSIZE 1024

//checks if a string contains another
int contains(char *string, char *pattern)
{
    int sl= 0, pl = 0, s = 0;
    char *ps;
    sl = strlen(string);
    pl = strlen(pattern);
    ps = string;

    for(; s < sl; s++)
    {
        if (strncmp(ps, pattern, pl) == 0)
        {
            return 1;
        }
        ps++;
    }
    return 0;
}

//main function for grep argv[1] is pattern
int main(int argc, char *argv[ ])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>>KAOS GREP<<<<<<<<<<<<<<<<<<<<<<<<<<\n\r");
    int fd, afd, in = 0, out, outfd, i, m, n, len, change, cont = 0;
    char buf[BLKSIZE] = "", c = 0, mytty;
    STAT fs;
    fd = 0; // default to stdin
    afd = 0;
    if (argc > 2){
        close(0);
        fd = open(argv[2], O_RDONLY);
        in = 1;
        if (fd < 0) exit(1); // file couldnt be open, quits
    }
    if(argc < 2)
        exit(0);

    //printi(fd);
    // checks if the file has been redirected
    afd = redirection();
    out = isfile(1);

    fstat(1, &fs);
    outfd = fs.st_dev;

    //printi(in);
    //printi(out);
    i = 0;
    while (n = read(fd, &c, 1)){
        if (in == 0) // if from stdin
        {
            if (afd == 0) // if more than 3 redirections
            {
                if (c == 4)
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
                    if (contains(buf, argv[1]))
                        write(1, buf, len);
                    i = 0;
                }
                else
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
                if (c == '\n')
                {
                    buf[i++] = c;
                    buf[i] = '\0';
                    if (contains(buf, argv[1]))
                    {
                        write(1, buf, (int)strlen(buf));

                        if (out == 0)
                            write(2, &cr, 1);
                    }
                    i = 0;
                }
                else if (c == '\r' && out == 0) //doesnt pass on '\r' values
                    continue;
                else                //all other characters are written
                    buf[i++] = c;
            }
        }
        else
        {
            char cr = '\r';
            if (c == '\n')
            {
                buf[i++] = c;
                buf[i] = '\0';
                if (contains(buf, argv[1]))
                {
                    write(1, buf, (int)strlen(buf));
                    if (out == 0)
                        write(1, &cr, 1);
                }
                i = 0;
            }
            else
                buf[i++] = c;
        }
    }
}