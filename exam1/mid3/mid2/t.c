int color;
char *str = "Nothing to see here";

#include "type.h"
#include "uart.c"
#include "string.c"
#include "queue.c"  // use provided queue.obj  during linking
#include "kbd.c"    // use provided kbd.obj    during linking
#include "vid.c"
#include "timer.c"
#include "exceptions.c"
#include "kernel.c"
#include "wait.c"
#include "pipe.c"   //pipe implementation

PIPE *kpipe;        // global PIPE pointer

int pipe_writer() // pipe writer task code
{
   struct uart *up = &uart[0];
   int i = 0;
   char line[128];
   while(1)
   {  
      change_colour();
      ufprintf(up, "Enter a line for task1 to get : ");
      printf("task%d waits for line from UART0\n", running->pid);
      ugets(up, line);
      uprints(up, "\r\n");
      if(i == 0)
      {
         tswitch();
         i++;
      }
      printf("task%d writes line=[%s] to pipe\n", running->pid, line);
      //printf("KPIPE STATUS (0 Free 5 BUSY): %d\n%d", kpipe->status, strlen(line));
      write_pipe(kpipe, line, strlen(line));
   }
}

int pipe_reader()    // pipe reader task code
{
   char line[128];
   int i, n;
   while(1)
   {
      change_colour();
      printf("task%d reading from pipe\n", running->pid);
      printf("\nline: %s\n", line);
      n = read_pipe(kpipe, line, 20);
      printf("task%d read n=%d bytes from pipe : [", running->pid, n);
      for (i=0; i<n; i++)
         kputc(line[i]);
      printf("]\n");
   }
}

void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}
int kprintf(char *fmt, ...);
void timer_handler();
TIMER *tp;

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  
    if (vicstatus & 0x80000000){
       if (sicstatus & 0x08){
          kbd_handler();
       }
    }
}

int body();

int main()
{ 
   color = WHITE;
   row = col = 0; 

   fbuf_init();
   //uart_init();
   kbd_init();
   //pipe_init();              //Initiate PIPEs
   //kpipe = create_pipe();    //create global kpipe

   /* enable timer0,1, uart0,1 SIC interrupts */
   VIC_INTENABLE |= (1<<4);  // timer0,1 at bit4 
   VIC_INTENABLE |= (1<<5);  // timer2,3 at bit5 
   VIC_INTENABLE |= (1<<31); // SIC to VIC's IRQ31

   /* enable KBD IRQ */
   SIC_ENSET = 1<<3;  // KBD int=3 on SIC
   SIC_PICENSET = 1<<3;  // KBD int=3 on SIC

   timer_init();
   tp = &timer;
 
   kprintf("Welcome to WANIX in ARM\n");
   init();
   kfork((int)body, 1);
   kprintf("P0 kfork tasks\n");
   printf("Starting timer!\n");
   timer_start();
   printf("Timer has started!\n");
   while(1){
     if (readyQueue)
        tswitch();
   }
}
