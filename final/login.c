/****************** Algorithm of login *******************/
// login.c : Upon entry, argv[0]=login, argv[1]=/dev/ttyX
#include "ucode.c"
int in, out, err;
int fd;

char uname[128], upassword[128], whattty[64], t[64];
char pline[64], mytty[64], *pname[8], tknz[8][64];

void tokenize(char *line)
{
    char *ptr = line;
    for(int i=0; i < 7; i++)
    {
        char *temp;
        temp = ptr;
        while(*ptr != ':' && *ptr != '\0')
        {
            //printf("%c", *ptr);
            *ptr++;
        }
        *ptr = '\0';
        *ptr++;
        strcpy(tknz[i], temp);
    }
}

int getfc(int file)
{
   int c, n;
   n = read(file, &c, 1);

   if (n==0 || c==4 || c==0 ) return EOF;  
                                
   return (c&0x7F);
}

// getline() does NOT show the input chars AND no cooking: 
// for reditected inputs from a file which may contain '\b' chars

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

main(int argc, char *argv[])
{
    //close file descriptors 0,1,2 inherited from INIT.
    close(0); close(1); close(2);

    //open argv[1] 3 times as in(0), out(1), err(2).
    strcpy(mytty, argv[1]);

    in  = open(mytty, 0);
    out = open(mytty, 1);
    err = open(mytty, 1);

    // record mytty name string in PROC.tty
    settty(mytty);

    printf("\nlogin: tty=%s\n", mytty);
    printf("KAOSLOGIN: open %s as stdin = %d, stdout = %d, stderr = %d\n",
            mytty, in, out, err);

    signal(2, 1);

    //open /etc/passwd file for READ;
    while(1){
        print2f("\n\rlogin:");
        gets(uname);
        print2f("password:");
        close(1);
        gets(upassword);
        out = open(mytty, 1);
        print2f("\n");

        int pswdFile = open("/etc/passwd", 0);

        while(getfline(line, pswdFile)){
            //tokenize user account line
            tokenize(line);
            // checks if the user name and password match the one listed in the file
            if(strcmp(tknz[0], uname) == 0 && strcmp(tknz[1], upassword) == 0)
            {
                //change uid, gid to user's uid, gid
                chuid(atoi(tknz[3]), atoi(tknz[2]));
                //change cwd to user's home DIR
                chdir(tknz[5]);
                //close opened /etc/passwd file
                close(pswdFile);
                //exec to program in user account (sh in all cases)
                tknz[6][strlen(tknz[6]) - 1] = '\0';
                char prog[64] = "../bin/";
                kstrcat(prog, tknz[6]);
                exec(prog);
            }
        }
        print2f("login failed, try again\n\r");
        close(pswdFile);
    }
}