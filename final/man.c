/***************************************************************/
//Konstantin Shvedov
/****************************MAN.C******************************/
// Manual page for all funtions implemented
/***************************************************************/
#include "ucode.c"
void ls(void);
void cat(void);
void more(void);
void grep(void);
void l2u(void);
void cp(void);

// asks the user if no function given and then calls function
// to print description
int main(int argc, char *argv[])
{
    char manPage[64];
    if (argc == 1)
    {
        print2f("What manual page do you want?\n\r");
        gets(manPage);
    }
    else if (argc == 2)
        strcpy(manPage, argv[1]);
    else
        exit(0);
    
    print2f("\n\r");

    if(!strcmp(manPage, "ls"))
        ls();
    else if(!strcmp(manPage, "cat"))
        cat();
    else if(!strcmp(manPage, "more"))
        more();
    else if(!strcmp(manPage, "grep"))
        grep();
    else if(!strcmp(manPage, "l2u"))
        l2u();
    else if(!strcmp(manPage, "cp"))
        cp();
    else
        print2f("Page doesn't exist\n\r");
    print2f("\n\r");
    exit(0);
}

void ls(void)
{
    print2f("NAME\n\r");
    print2f("\tls - list directory contents\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tls [FILE]\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tList  information  about the FILEs (the current directory by default).\n\r");
}

void cat(void)
{
    print2f("NAME\n\r");
    print2f("\tcat - concatenate files and print on the standard output\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tCAT [FILE]\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tConcatenate FILE(s) to standard output.\n\r\n\r");
    print2f("\tWith no FILE, read standard input.\n\r");
}

void more(void)
{
    print2f("NAME\n\r");
    print2f("\tmore - file perusal filter for crt viewing\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tmore [FILE]\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tmore  is  a filter for paging through text one screenful at a time.\n\r");
}

void grep(void)
{
    print2f("NAME\n\r");
    print2f("\t grep - print lines matching a pattern\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tgrep patter [FILE]\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tgrep  searches  for PATTERN in each FILE.  If no FILE\n\r");
    print2f("\tgrep  reads standard input. By default, grep prints the matching lines.\n\r");
    exit(0);
}

void l2u(void)
{
    print2f("NAME\n\r");
    print2f("\tl2u - convert lower case to upper case\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tl2u [FILE1 FILE2]\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tl2u coppies file and changes all text to uppercase. If no input\n\r");
    print2f("\tstandard input is used\n\r");
}

void cp(void)
{
    print2f("NAME\n\r");
    print2f("\tcp - copy files and directories\n\r");
    print2f("SYNOPSIS\n\r");
    print2f("\tcp SOURCE DEST\n\r");
    print2f("DESCRIPTION\n\r");
    print2f("\tCopy SOURCE to DEST.\n\r");
}