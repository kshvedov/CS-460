#include "ucode.c"

#define BLKSIZE 1024

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
    int fd, i, m, n, pgl = 23;
    char c;
    fd = 0; // default to stdin

    if (argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) exit(1);
    }

    writeLine(fd, pgl);

    while (1)
    {
        c = getc();
        if (c == '\r')
            writeLine(fd, 1);
        else if (c == ' ')
            writeLine(fd, pgl);
        
    }
    

}