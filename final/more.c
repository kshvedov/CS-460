#include "ucode.c"

#define BLKSIZE 1024

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

int writeLine(int fd, int lines)
{
    int n, m;
    char buf[BLKSIZE];

    for(int i = 0; i < lines; i++)
    {
        if(n = getfline(buf, fd))
        {
            buf[n] = '\r';
            n++;
            m = write(1, buf, n);
            if (m == 0)
                exit(0);
        }
        if (n == 0)
            exit(0);
    }
}

int main(int argc, char *argv[ ])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>>>KAOS MORE<<<<<<<<<<<<<<<<<<<<<<<<\n\r");
    int fd, i, m, n, pgl = 23, k = 1, in = 0, out, ttyfd;
    char c, mytty[64], c2, c3;
    fd = 0; // default to stdin

    if (argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        in = 1;
        if (fd < 0) exit(1);
    }
    else
        k = 2;

    writeLine(fd, pgl*k);
    
    gettty(mytty);
    ttyfd = open(mytty, O_RDONLY);
    
    out = isfile(1);

    while (1)
    {
        n = read(ttyfd, &c, 1);

        if (c == 4)
            exit(0);
        else if (c == '\r')
            writeLine(fd, 1 * k);
        else if (c == ' ')
            writeLine(fd, pgl * k);
    }
}