/***************************************************************/
//Konstantin Shvedov
/**************************LOGIN.C******************************/
// This is the login process, first thing called when a new port
// is activated
/***************************************************************/

#include "ucode.c"
#include "xtr.c"
int in, out, err;
int fd;

int p = 1319;
int q = 6997;
char *salt1 = "y5r'ttFm{!~](LZE";
char *salt2 = "-?}Wrcq]Q8TfX_aU";

char uname[128], upassword[128], t[64];
char pline[64], mytty[64], *pname[8], tknz[8][64];

// this is the encryption method used to encrypt the
// password, and int is stored in the password file
// which is compared to an attempted hash password
unsigned int encrypt(char *pass)
{
    unsigned int hash = p*q;
    char s[128] = "";
    char c;
    strcpy(s, salt1);
    strcat(s, pass);
    strcat(s, salt2);
    while (c = *pass++)
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// a new rokenize function just for seperating
// informtation in the password file
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


// Main loging process
// argv[0]=login, argv[1]=/dev/ttyX
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

    // print some essential info
    printf("\nlogin: tty=%s\n", mytty);
    printf("KAOSLOGIN: open %s as stdin = %d, stdout = %d, stderr = %d\n",
            mytty, in, out, err);

    //signal handler is installed for ctrl-c
    signal(2, 1);

    // infinite login process is launched till credentials match
    while(1){
        print2f("\n\rlogin:");
        gets(uname);
        print2f("password:");

        // to imitate linux output is closed so password isn't seen
        // when types
        close(1);
        gets(upassword);

        //output is reopened to allow all other printing
        out = open(mytty, 1);

        print2f("\n\r");

        // open /etc/passwd file for READ;
        int pswdFile = open("/etc/passwd", 0);

        while(getfline(line, pswdFile)){    // gets lines of file till EOF
            //tokenize user account line
            tokenize(line);
            // checks if the user name and password match the one listed in the file
            if(strcmp(tknz[0], uname) == 0 && atoi(tknz[1]) == encrypt(upassword))
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
        print2f("\rlogin failed, try again\n\r");
        close(pswdFile);
    }
}