/***************************************************************/
//Konstantin Shvedov
/***************************INIT.C******************************/
// This file containts all the initiallisation code needed to
// launch 3 login process:
// One on Consle, one on ttyS0 and one on ttyS1
/***************************************************************/

#ifndef INIT_C
#define INIT_C

#include "ucode.c"

int console, serial0, serial1;
int in, out, err;
char t[64];

// Concatinates the port onto "login " so it can be passed
// on to the exec function which launches the port
void login(char *s)
{
    char temp[64] = "login ";
    kstrcat(temp, s);
    exec(temp);
}

// This is the P1 process that keeps all the logins up
// if any one of turns into a zombie, it launches a new
// replacement one and then continues waiting
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

// This is the main portion of init
// P1 forks 3 process's and executes a login for there ports
// P1 then waits for them to become zombies in parent() 
int main(int argc, char *argv[ ])
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