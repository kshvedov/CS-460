#ifndef INIT_C
#define INIT_C

#include "ucode.c"
#include "kaostypes.h"

int console, serial0, serial1;
int in, out, err;
char t[64];

void login(char *s)
{
    char temp[64] = "login ";
    kstrcat(temp, s);
    exec(temp);
}

void parent()              // P1's code
{
    int pid, status;
    while(1){
        printf("INIT : wait for ZOMBIE child\n");
        pid = wait(&status);

        if (pid==console) // if console login process died
        {
            printf("INIT: forks a new console login\n");
            console = fork(); // fork another one
            if (console)
                continue;
            else
                login("/dev/tty0"); // new console login process
        }

        if (pid==serial0) // if serial0 login process died
        {
            printf("INIT: forks a new serial0 login\n");
            serial0 = fork(); // fork another one
            if (serial0)
                continue;
            else
                login("/dev/ttyS0"); // new serial0 login process
        }

        if (pid==serial1) // if serial1 login process died
        {
            printf("INIT: forks a new serial1 login\n");
            serial1 = fork(); // fork another one
            if (serial1)
                continue;
            else
                login("/dev/ttyS1"); // new serial1 login process
        }

        printf("INIT: I just buried an orphan child proc %d\n", pid);
    }
}

int main(void)
{
    int pid;
    pid = getpid();
    in = open("/dev/tty0", 0);
    out= open("/dev/tty0", 1);
    //printf("Got PID: %d\n", pid);
    printf("\nIN: %d OUT: %d\n", in, out);

    printf("\nINIT forks %d login processes\n", pid);

    console = fork();
    printf("CONSOLE: %d\n", console);
    if (console)
    {
        //gets(t);
        serial0 = fork();
        printf("SERIAL0: %d\n", serial0);
        if (serial0)
        {
            //gets(t);
            serial1 = fork();
            printf("SERIAL1: %d\n", serial1);
            if (serial1)
                parent();
            else
            {
                //pause(1);
                login("/dev/ttyS1");
            }
        }
        else
        {
            //pause(1);
            login("/dev/ttyS0");
        }
    }
    else
        login("/dev/tty0");

    return 0;
}

#endif