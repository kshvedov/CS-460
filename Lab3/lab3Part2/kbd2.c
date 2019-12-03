#include "keymap"

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

#define CAPSLOCK         0x3A
#define LSHIFT           0x2A
#define RSHIFT           0x36
#define CONTROL          0x1D

typedef volatile struct kbd{
  char *base;
  char buf[128];
  int head, tail, data, room;
}KBD;

volatile KBD kbd;

// state variables
int shift;		    // left and right shift key state 
int control;		// control key state 
int capslock;		// caps lock key state 
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

  if (code & 0x80){ // key release: ONLY catch release of shift, control
    code &= 0x7F;   // mask out bit 7
    if (code == LSHIFT || code == RSHIFT)
      shift = 0;    // released the shift key
    if (code == CONTROL)
      control = 0;  // released the Control key
    return;
  }

  // from here on, must be key press 
  if (code == LSHIFT || code == RSHIFT){
    shift = 1;
    return;
  }
  if (code == CONTROL){
    control = 1;
    return;
  }

  if (code == 0x3A){
    // handle capslock
    return;
  }

  c = (shift ? sh[code] : unsh[code]);

  /* (1). write code to handle caps lock key *******/
  

  /* (2). write code to recognize Control-C key *******/

  if (control-c){    // Control-C keys on PC, these are 2 keys
     printf("Control-c key\n");
     c = '\n'; // force a line, let proc handle #2 signal when exit Kmode
  }
  
  /* (3). write code to recognize Control-D key *******/
  if (control-d)){  // Control-D, these are 2 keys
     c = 4;   // Control-D
  }

  // Store char in buf[] for task to get 

  if (kp->data == 128)  // buf FULL, ignore current key
    return;

  kp->buf[kp->head++] = c;
  kp->head %= 128;

  kp->data++;
}

int kgetc()
{
  char c;
  KBD *kp = &kbd;

  while(kp->data == 0);

  lock();
    c = kp->buf[kp->tail++];
    kp->tail %= 128;
    kp->data--; kp->room++;
  unlock();
  return c;
}