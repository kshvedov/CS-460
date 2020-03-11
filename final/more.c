/***************************************************************/
//Konstantin Shvedov
/****************************MORE.C*****************************/
// More function, based of cat in many way, will allow a person
// to go through text using space (screen) or enter (line)
/***************************************************************/
#include "ucode.c"
#include "xtr.c"

#define BLKSIZE 1024

int getmfline(char *s, int file)
{
  int c;  
  char *cp = s;
  
  c = getfc(file);

  while ((c != EOF) && (c != '\n')){
    if (c != '\r')
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
        if(n = getmfline(buf, fd))
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
    int fd, i, m, n, pgl = 23, in = 0, out, ttyfd;
    char c, mytty[64], c2, c3;
    fd = 0; // default to stdin

    if (argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        in = 1;
        if (fd < 0) exit(1);
    }
    writeLine(fd, pgl);
    
    //close(0);
    gettty(mytty);
    ttyfd = open(mytty, O_RDONLY);
    
    out = isfile(1);

    while (1)
    {
        n = read(ttyfd, &c, 1);

        if (c == 4)
            exit(0);
        else if (c == '\r')
            writeLine(fd, 1);
        else if (c == ' ')
            writeLine(fd, pgl);
    }
}