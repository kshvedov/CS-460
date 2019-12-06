#include "ucode.c"

#define BLKSIZE 1024

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

int getfc(int file)
{
   int c, n;
   n = read(file, &c, 1);
   if (n==0 || c==4 || c==0 ) return EOF;  
                                
   return (c&0x7F);
}

int getfline(char *s, int file)
{
  int c;  
  char *cp = s;
  
  c = getfc(file);

  while ((c != EOF) && (c != '\r') && (c != '\n')){
    *cp++ = c;
     c = getfc(file);
  }
  if (c==EOF) return 0;

  *cp++ = c;         // a string with last char=\n or \r
  *cp = 0;    
  //printf("getline: %s", s); 
  return strlen(s);  // at least 1 because last char=\r or \n
}

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