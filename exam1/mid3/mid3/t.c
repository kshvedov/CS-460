int color;
int startT = 0;
//PIPE *kpipe;        // global PIPE pointer

#include "type.h"
#include "string.c"
#include "queue.c"  // use provided queue.obj  during linking
#include "kbd.c"    // use provided kbd.obj    during linking
#include "vid.c"
#include "exceptions.c"
#include "timer.c"
#include "kernel.c"
#include "tqe.c"
#include "wait.c"


void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}
int kprintf(char *fmt, ...);
void timer_handlers();

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  

    // VIC status BITs: timer0,1=4, uart0=13, uart1=14
   if (vicstatus & (1<<4)){   // bit4=1:timer0,1
      timer_handler(0);       // timer0 only
   }
   if (vicstatus & (1<<31)){  // PIC.bit31= SIC interrupts
      if (sicstatus & (1<<3)){// SIC.bit3 = KBD interrupt
         kbd_handler();
      }
   }
   /*if (vicstatus & 0x80000000){
      if (sicstatus & 0x08){
         kbd_handler();
      }
   }*/
}

int body();

int main()
{ 
   color = WHITE;
   row = col = 0; 

   fbuf_init();
   kbd_init();
   
   /* enable timer0,1, uart0,1 SIC interrupts */
   VIC_INTENABLE  = 0;
   VIC_INTENABLE |= (1<<4);  // timer0,1 at bit4 
   VIC_INTENABLE |= (1<<5);  // timer2,3 at bit5 
   VIC_INTENABLE |= (1<<31); // SIC to VIC's IRQ31

   /* enable KBD IRQ */
   SIC_ENSET = 1<<3;  // KBD int=3 on SIC
   SIC_PICENSET = 1<<3;  // KBD int=3 on SIC
   /*SIC_INTENABLE = 0;
   SIC_INTENABLE |= (1<<3);*/   // KBD int=SIC.bit3

   timer_init();              // initialize timer
   timer_start(0);            // start timer
   kbd_init();                // initialize keyboard driver
   tqe_init();
 
   kprintf("Welcome to WANIX in Arm\n");
   init();
   kfork((int)body, 1);
   while(1){
     if (readyQueue)
        tswitch();
   }
}
