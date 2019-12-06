#include "ucode.c"

#define BLKSIZE 4096

int redirection()
{
	STAT fs, fs2;
	// Stat stdout
	fstat(0, &fs);
    fstat(1, &fs2);
    //printi(fs.st_dev);


    if (fs.st_dev == fs2.st_dev && fs.st_dev > 0)
        return 0;
    return 1;
}

int isfile(int fd)
{
    STAT fs;
	// Stat stdout
	fstat(fd, &fs);

    if(((fs.st_mode & 0xF000) == 0x8000) ||
		((fs.st_mode & 0xF000) == 0x4000) ||
		((fs.st_mode & 0xF000) == 0xA000))
        return 1;
    return 0;
}

int main(int argc, char *argv[ ])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>KAOS CAT ROAR<<<<<<<<<<<<<<<<<<<<<<\n\r");
    int fd, afd, in = 0, out, outfd, i, m, n, len, change;
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

    fstat(1, &fs);
    outfd = fs.st_dev;

    //printi(in);
    //printi(out);
    i = 0;
    while (n = read(fd, &c, 1)){
        if (in == 0)
        {
            if (afd == 0)
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
            if(out == 0)
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