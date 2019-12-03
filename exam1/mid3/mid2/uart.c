// uart.c file

#define DR   0x00
#define FR   0x18

#define RXFE 0x10
#define TXFF 0x20

BASE = 10;

typedef struct uart{
  char *base;
  int n;
}UART;

UART uart[4];

int uart_init()                         //UART initialization function
{
    /*ARM versatilepb's 4 UARTs are at the base addresses
        0x101F1000, 0x101F2000, 0x101F3000, 0x10009000
    Write code to initialize the 4 UART structs.*/
    int i; UART *up;
    for (i=0; i<4; i++){                //uart0 to uart2 are adjacent
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000);
        up->n = i;
    }
    uart[3].base = (char *)(0x10009000);//uart3 at 0x10009000
}

int ugetc(UART *up)                     //input a char from UART pointed by up
{
    /*up points at a UART struct;
    Write code to return an input char from the UART*/
    while (*(up->base+FR) & RXFE);      // loop if UFR is REFE
    return *(up->base+DR);              // return a char in UDR
}

int uputc(UART *up, char c)
{
    /*up points at a UART struct;
    Write code to output a char to the UART*/
    while (*(up->base+FR) & TXFF);      // loop if UFR is TXFF
    *(up->base+DR) = c;                 // write char to data register
}

int ugets(UART *up, char s[ ])
{
    /*Write code to input a string from a UART*/
    while ((*s = (char)ugetc(up)) != '\r') {
        uputc(up, *s++);
    }
    *s = 0;
}

int uprints(UART *up, char *s)
{
  /*Write code to print a string s to UART*/
    while (*s)
        uputc(up, *s++);
}

char *ctable = "0123456789ABCDEF";

int urpu(UART *up, u32 x)
{  
   char c;
   if (x)
   {
      c = ctable[x % BASE];
      urpu(up, x / BASE);
      uputc(up, c);
   }
}

int uprintu(UART *up, u32 x)
{
   (x == 0)? uputc(up, '0') : urpu(up, x);
   uputc(up, ' ');
}

int uprintd(UART *up, int x)
{
   if(x == 0)
   {
      uputc(up,'0');
   }
   else if (x < 0)
   {
      uputc(up,'-');
      x = -x;
   }
   urpu(up, x);
   uputc(up, ' ');
}

int uprintx(UART *up, u32 x)
{
   uputc(up, '0'); uputc(up,'x');
   BASE = 16;
   uprintu(up, x);
   BASE = 10;
}

// major work:
ufprintf(UART *up, char *fmt, ...)
{
    /*Write code to print char, string, unsigned integer, integer
                    by   %c     %s           %x           %d*/
    char *cp = fmt;
    char tp[10];
    int *ip = (int *)&fmt + 1;
    for(int i = 0; cp[i] != '\0'; i++)
    {
        if(cp[i] == '\n')
        {
            uputc(up, '\n');
        }
        else if(cp[i] == '\t')
        {
            uputc(up, '\t');
        }
        else if(cp[i] != '%')
        {
            uputc(up, cp[i]);
        }
        else
        {
            i++;
            switch(cp[i])
            {
                case 'c':
                uputc(up, *ip);
                break;
                case 's':
                uprints(up, *ip);
                break;
                case 'd':
                uprintd(up, *ip);
                break;
                case 'x':
                uprintx(up, *ip);
                break;
                case 'u':
                uprintu(up, *ip);
                break;
            }
            ip++;
        }
    }
}