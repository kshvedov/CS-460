/************* kbd.c file **************/

#include "keymap"
#include "keymap2"

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

typedef volatile struct kbd{
  char *base;
  char buf[128];
  int head, tail, data, room;
}KBD;

volatile KBD kbd;
int count;

int kbd_init()
{
  char scode;
  KBD *kp = &kbd;
  kp->base = (char *)0x10006000;
  *(kp->base + KCNTL) = 0x10; // bit4=Enable bit0=INT on
  *(kp->base + KCLK)  = 8;
  kp->head = kp->tail = 0;
  kp->data = 0; kp->room = 128;
  count = 0;  // number of KBD interrupts
}

void kbd_handler()
{
  u8 scode, c;
  KBD *kp = &kbd;
  color = RED;
  scode = *(kp->base + KDATA);
  count++;
  if (scode & 0x80)       // ignore key releases
    return;
  c = ltab[scode];        // map scan code to ASCII
  if (c != '\r')
    printf("kbd interrupt: c=%x %c\n", c, c);
  kp->buf[kp->head++] = c;
  kp->head %= 128;        // enter key into CIRCULAR buf[ ]
  kp->data++; kp->room--; // update counters
}

int kgetc()               // main program calls kgetc() to return a char
{
  char c;
  KBD *kp = &kbd;
  unlock();               // enable IRQ interrupts
  while(kp->data <= 0);   // wait for data; READONLY
    lock();                 // disable IRQ interrupts
    c = kp->buf[kp->tail++];// get a c and update tail index
    kp->tail %= 128;
    kp->data--; kp->room++; // update with interrupts OFF
  unlock();               // enable IRQ interrupts
  return c;
}

int kgets(char s[ ])      //gets a string from KBD
{
  char c;
  while((c=kgetc()) != '\r'){
    *s++ = c;
  }
  *s = 0;
  return strlen(s);
}
