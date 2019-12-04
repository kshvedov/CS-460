#include "ucode.c"
int BLKSIZE = 1024, chng = 0, chng2 = 0;
char loc[1024];
char dUse[1024] = "";

int opendir(pathaname)
{
    return open(pathname, O_RDONLY);
}

void fancyPrint2f(char *str, char *end, int len)
{
    int s, e;
    char final[256] = "";
    s = strlen(str);
    e = strlen(end);

    strcpy(final, str);

    if((s+e) >= len)
    {
        strcat(final, end);
    }
    else
    {
        int t;
        t = len - (s + e);
        while (t > 0)
        {
            strcat(final, " ");
            t--;
        }
        strcat(final, end);
    }
    print2f(final);
}

char* itoa(int x, char **s)
{
    char num[32] = "", temp[32]= "";
    int i = 0, j = 0;
    s[0] = '\0';
    if (x==0){
       strcpy(s, "0");
       return "0";
    }
    if (x < 0){
       num[i] = '-';
       i++;
       x = -x;
    }
    while (x != 0)
    {
        temp[j] = ctable[x%10];
        j++;
        x = (int)x/10;
    }
    j--;
    for(; j >= 0; j--)
    {
        num[i] = temp[j];
        i++;
    }
    num[i] = '\0';
    strcpy(s, num);

    return num;
}

//prints all details about a file
void ls_file(char *fname)
{
    //printf("NAME:%s\n", fname);
    STAT fstat, *sp = &fstat;
    int r, i;
    
    if(strlen(loc) > 0 && chng == 0)
    {
        chng = 1;
        strcat(loc, fname);
    }

    r = stat(loc, sp);
    if (r != 0)
    {
        printf("File \"%s\" doesn't exist\n\r", fname);
        return;
    }
    
    if ((sp->st_mode & 0xF000) == 0x4000)      // if (S_ISDIR())
        print2f("d");
    else if ((sp->st_mode & 0xF000) == 0x8000) // if (S_ISREG())
        print2f("-");
    else if ((sp->st_mode & 0xF000) == 0xA000) // if (S_ISLNK())
        print2f("l");
    for(int i = 8; i >=0; i--)
    {
        if (sp->st_mode & (1 << i))
        print2f("r");
        else print2f("-");
        i--;
        if (sp->st_mode & (1 << i))
        print2f("w");
        else print2f("-");
        i--;
        if (sp->st_mode & (1 << i))
        print2f("x");
        else print2f("-");
    }
    char s[32] = "";

    itoa(sp->st_nlink, s);
    fancyPrint2f("", s, 4);
    itoa(sp->st_gid, s);
    fancyPrint2f("", s, 4);
    itoa(sp->st_uid, s);
    fancyPrint2f("", s, 4);
    itoa(sp->st_size, s);
    fancyPrint2f("", s, 10);
    printf(" %s", fname);
    print2f(" \n\r");
}

//list all files at location specified and then sends to ls_file
int ls_dir(char *dname)
{
    char buffer[1024];
    char name[256];             // EXT2 filename: 1-255 chars
    DIR *dp;

    int fd = open(dname, O_RDONLY);    // opendir() syscall
    read(fd, buffer, 1024);

    char *cp = buffer;
    dp = (DIR*)buffer;

    while(cp < 1024 + buffer)
    {
        strcpy(name, dp->name);
        name[dp->name_len] = '\0';

        ls_file(name);

        cp += dp->rec_len;
        dp = (DIR*)cp;
    }
}

int main(int argc, char *argv[])
{
    print2f("\r>>>>>>>>>>>>>>>>>>>>>>>>>>KAOS LS<<<<<<<<<<<<<<<<<<<<<<<<<<\n\r");
    struct stat mystat, *sp;
    int r;
    char *s;
    char filename[1024], cwd[1024];

    s = argv[1];                // ls [filename]

    if (argc == 1)              // no parameter: ls CWD
        s = "./";
    sp = &mystat;

    if ((r = stat(s, sp)) < 0)  // stat() syscall
    {
        print2f("There was an error with ls with:\n\r");
        printf("\"%s\"\n\r", s);
        exit(1);
    }

    strcpy(filename, s);
    strcpy(loc, s);

    if (s[0] != '/'){           // filename is relative to CWD
        getcwd(cwd);            // get CWD path
        strcpy(filename, cwd);
        //strcat(filename, "/");
        strcat(filename,s);     // construct $CWD/filename
    }

    if ((sp->st_mode & 0xF000) == 0x4000)
        ls_dir(filename);       // list DIR
    else
    {
        ls_file(filename);      // list single file
    }
}