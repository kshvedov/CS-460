/***************************************************************/
//Konstantin Shvedov
/****************************XTR.C******************************/
// Extra functions that I used in many different places
/***************************************************************/
#include "ucode.c"

//gets character from a specific file
int getfc(int file)
{
   int c, n;
   n = read(file, &c, 1);
   if (n==0 || c==4 || c==0 ) return EOF;  
                                
   return (c&0x7F);
}

// gets line from specific file
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

// this function allows cat and grep to be piped more than 3 times into
// each other
int redirection()
{
	STAT fs, fs2;
	// Stat stdout
	fstat(0, &fs);
    fstat(1, &fs2);

    if (fs.st_dev == fs2.st_dev && fs.st_dev > 0)
        return 0;
    return 1;
}

// This function checks if the fd is a file or stdin
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