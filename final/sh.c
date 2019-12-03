/********************* sh.c file ***********************/
#include "ucode.c"
// user commands and syscall interface
char cmdline[128], cmd[64];
char cmdlineb[128];

int newCommand()
{
    char dir[256];
    print2f("\r***********************************************************\n");
    print2f("\r                      KAOS SHELL\n");
    print2f("\r***********************************************************\n");
    //printf("[ls|cd|pwd|mkdir|rmdir|creat|link|unlink|symlink|readlink|stat|chmod|touch|quit]\n");
    //printf("[open|close|lseek|pfd|read|write|cat|cp|mv|mount|unmount]\n");
    getcwd(dir);
    printf("\rUser@kaos: %s$: ", dir);
    gets(cmdline);
    print2f("\r***********************************************************\n");
    
    int len = strlen(cmdline);
    if(len > 1)
    {
        if (cmdline[len-1] == '\n')
            cmdline[len-1] = '\0';
    }
    strcpy(cmdlineb, cmdline);
}

int tokenizecmd()
{
    token(cmdline);

    return argc;
}

int checkPipe()
{
    int opipe = 0;
    for(int i = 0; i < argc; i++)\
    {
        if(!strcmp(argv[i],"|"))
            opipe++;
    }
    return opipe;
}

void execCmd(char** input)
{
    int i = 0, j = 0;
    char excmd[128] = {0};
    for(i = 0; input[i] != 0; i++)
    {
        if (strcmp(input[i], "<") == 0) // input
        {
            close(0);
            int fd = open(input[i + 1], O_RDONLY);
            input[i] = 0;
        }
        else if (strcmp(input[i], ">") == 0) // output
        {
            close(1);
            int fd = open(input[i + 1], O_WRONLY | O_CREAT);
            input[i] = 0;
        }
        else if (strcmp(argv[i], ">>") == 0) // output and append
        {
            close(1);
            int fd = open(input[i + 1], O_WRONLY | O_APPEND | O_CREAT);
            input[i] = 0;
        }
    }

    for(j = 0; j < i; j++)
    {
        if (input[j] != 0)
        {
            strcat(excmd, input[j]);
            if ((j+1) < i)
                strcat(excmd, " ");
        }
    }
    exec(excmd);
}

void execPipe(int pipeNum, char** input)
{
    char *left[1024] = { 0 };
    char *right[1024] = { 0 };
    int i = 0;
    int j = 0;

    while(strcmp(input[i], "|") != 0)
    {
        left[i] = input[i];
        i++;
    }
    i++;
    while(argv[i] != 0)
    {
        right[j] = input[i];
        i++;
        j++;
    }

    print2f("Left Side: ");
    for (int k = 0; left[k] != 0; k++)
    {
        printf("%s ", left[k]);
    }
    print2f("\n\r");

    print2f("Right Side: ");
    for (int k = 0; right[k] != 0; k++)
    {
        printf("%s ", right[k]);
    }
    print2f("\n\r");

    pipeNum--;

    int pd[2], pid;
    pipe(pd);
    pid = fork();

    if(pid)//parent as pipe Reader
    {
        close(pd[1]);
        close(0);
        dup2(pd[0], 0);
        //close(pd[0]);
        if (pipeNum <= 0)
            execCmd(right);
        else execPipe(pipeNum, right);
    }
    else//child as pipe Writer
    {
        close(pd[0]);
        close(1);
        dup2(pd[1], 1);
        //close(pd[1]);
        execCmd(left);
    }
}

int execCmdL()
{
    int pid, status, opipe;

    strcpy(cmd, argv[0]);
    if (!strcmp(cmd,"exit"))
        exit(0);

    opipe = checkPipe();

    pid = fork();           // fork a child process to execute the cmd line

    if (pid)                // parent sh waits for child to die
        pid = wait(&status);
    else                    // child exec cmdline
    {
        if(!opipe)
        {
            execCmd(argv); // exec("cmd a1 a2 ... an");
        }
        else
        {
            execPipe(opipe, argv);
        }
    }
}

main(int argc, char *argv[ ])
{
    char t[64];
    int cmdNum;
    while(1){
        //display executable commands in /bin directory
        // prompt for a command line cmdline = "cmd a1 a2 .... an"
        newCommand();
        cmdNum = tokenizecmd();
        //printf("\rCommands inputed: %d\n", cmdNum);

        if (cmdNum > 0)
        {
            print2f("\r*********************EXECUTING COMMAND*********************\n\r");
            execCmdL();
            print2f("\r*********************COMMAND EXECUTED**********************\n\r");
        }
    }
    return 0;
}