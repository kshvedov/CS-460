/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "defines.h"
#include "uart.c"
#include "vid.c"

int color;
char *tab = "0123456789ABCDEF";
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/******** in uart.c file *******
typedef struct uart{
  char *base;
  int  n;
}UART;

UART uart[4];
*********************************/

extern char _binary_wsu_bmp_start;

int color;

int main()
{
    int x, size = 1;
    char c, *p;
    UART *up;

    uart_init();
    up = &uart[0];

    fbuf_init();

    p = &_binary_wsu_bmp_start;
    show_bmp(p, 0, 0, size); 

    while(1){
        ufprintf(up, "enter a key from this UART : ");
        ugetc(up);
        for (x=0; x<640*480; x++)
            fb[x] = 0x00000000;
        p = &_binary_wsu_bmp_start;
        size++;
        show_bmp(p, 0, 0, size);
    }
    while(1);   // loop here  
}