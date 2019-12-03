#include "keymap"

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

#define CAPSLOCK         0x58
#define LSHIFT           0x12
#define RSHIFT           0x56
#define CONTROL          0x14

typedef volatile struct kbd{
  char *base;
  char buf[128];
  int head, tail, data, room;
}KBD;

volatile KBD kbd;

// state variables
int shift;		      // left and right shift key state 
int control;		    // control key state 
int capslock;		    // caps lock key state 
int release;        // key release

int kbd_init()
{
  KBD *kp = &kbd;
  kp->base = (char *)0x10006000;
  *(kp->base + KCNTL) = 0x10; // bit4=Enable bit0=INT on
  *(kp->base + KCLK)  = 8;
  kp->head = kp->tail = 0;
  kp->data = 0; kp->room = 128;

  shift   = 0;
  control = 0;
  capslock = 0;
  release = 0;
}

void kbd_handler()
{
  unsigned char code, c;
  int i;
  KBD *kp = &kbd;

  code = *(kp->base + KDATA);
  //printf("code : %x", code);
  if (code == 0xF0)
    return;

  //if (code & 0x80){ // key release: ONLY catch release of shift, control
  //  code &= 0x7F;   // mask out bit 7
  if (release == 1)
  {
    if (code == LSHIFT || code == RSHIFT)
    {
      //printf("returned shift");
      shift = 0;    // released the shift key
    }
    if (code == CONTROL)
    {
      //printf("returned shift");
      control = 0;  // released the Control key
    }
    release = 0;
    return;
  }
  else 
  {
    release = 1;
  }

  /*if (release == 0)
    release = 1;
  else
  {
    release = 0;
    return;
  }*/

  // from here on, must be key press 
  if (code == LSHIFT || code == RSHIFT){
    shift = 1;
    return;
  }
  if (code == CONTROL){
    control = 1;
    return;
  }

  if (code == CAPSLOCK){
    if(capslock == 1)
    {
      capslock = 0;
    }
    else
    {
      capslock = 1;
    }
    return;
  }

  c = (shift ? utab[code] : ltab[code]);

  /* (1). write code to handle caps lock key */
  if (capslock == 1)
  {
    if (c >= 'A' && c <= 'Z')
      c += 'a' - 'A';
    else if (c >= 'a' && c <= 'z')
      c -= 'a' - 'A';
  }  

  /* (2). write code to recognize Control-C key */
  if (control == 1 && (c == 'C' || c == 'c')){    // Control-C keys on PC, these are 2 keys
    printf("Control-c key\n");
    c = '\n'; // force a line, let proc handle #2 signal when exit Kmode
  }
  
  /* (3). write code to recognize Control-D key */
  if (control == 1 && (c == 'D' || c == 'd')){  // Control-D, these are 2 keys
    printf("Control-d key\n");
    c = 4;   // Control-D
  }

  // Store char in buf[] for task to get 

  if (kp->data == 128)  // buf FULL, ignore current key
    return;

  printf("%c", c);

  kp->buf[kp->head++] = c;
  kp->head %= 128;

  kp->data++;
}

int kgetc()
{
  //printf("In kgetc\n");
  char c;
  KBD *kp = &kbd;

  unlock();
  while(kp->data <= 0);
  lock();
    c = kp->buf[kp->tail++];
    kp->tail %= 128;
    kp->data--; kp->room++;
  unlock();
  return c;
}

int kgets(char s[ ])      //gets a string from KBD
{
  //printf("In kgets\n");
  char c;
  while((c=kgetc()) != '\r'){
    *s++ = c;
  }
  *s = 0;
  return strlen(s);
}