/*******************************************************
*                  @t.c file                          *
*******************************************************/

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

char buf1[BLK], buf2[BLK];

int prints(char *s)
{
    while (*s) putc(*s++);
}

/*int gets(char *s)
{ 
    while ((*s = getc()) != '\r') putc(*s++);
	*s = 0;
}*/

//u16 NSEC = 2;

u16 getblk(u16 blk, char *buf)
{
    readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
}

u16 search(INODE *ip, char *name)
{
    char sbuf[BLK], temp[64], *cp;
    DIR *dp;
    u8 i;

    for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
        if (ip->i_block[i] == 0) break;

        getblk((u16)ip->i_block[i], sbuf);

        dp = (DIR *)sbuf;
        cp = sbuf;

        while(cp < sbuf + BLK){
            strncpy(temp, dp->name, dp->name_len);
            temp[dp->name_len] = 0;

            if (strcmp(temp, name)==0)
                return (u16)dp->inode;

            cp += dp->rec_len;
            dp = (DIR *)cp;
        }
    }
    error();
}

/*INODE *search(INODE *ip, char *name, u16 iblk)
{
    char sbuf[BLK], temp[64], *cp;
    DIR *dp;
    u8 i;

    for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
        if (ip->i_block[i] == 0) break;

        getblk((u16)ip->i_block[i], sbuf);

        dp = (DIR *)sbuf;
        cp = sbuf;

        while(cp < sbuf + BLK){
            strncpy(temp, dp->name, dp->name_len);
            temp[dp->name_len] = 0;

            if (strcmp(temp, name)==0)
            {
                i = (u16)dp->inode-1;
                getblk((u16)((i / 8) + iblk), buf2);
                ip = (INODE *)buf2 + (i % 8);
                return ip;
            }
            cp += dp->rec_len;
            dp = (DIR *)cp;
        }
    }
    error();
}*/

main()
{ 
    GD    *gp;
    INODE *ip;
    u16    i, iblk, ino;
    u32    *iblk2;

    //1. Write YOUR C code to get the INODE of /boot/mtx
    //   INODE *ip --> INODE
    //   if INODE has indirect blocks: get i_block[12] int buf2[  ]
    getblk((u16)2, buf1);

    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table;
    getblk((u16)iblk, buf1);             //gets first inode
    ip = (INODE *)buf1 + 1;         //ip-->root inode

    ino = search(ip, "boot")-1;
    getblk((u16)((ino / 8) + iblk), buf1);
    ip = (INODE *)buf1 + (ino % 8);

    ino = search(ip, "mtx")-1;
    getblk((u16)((ino / 8) + iblk), buf1);
    ip = (INODE *)buf1 + (ino % 8);

    /*ip = search(ip, "boot", iblk);
    ip = search(ip, "mtx", iblk);*/

    getblk((u16)ip->i_block[12], buf2);
    iblk2 = (u32*)buf2;

    setes(0x1000);                  // MTX loading segment = 0x1000

    //3. load 12 DIRECT blocks of INODE into memory beginning at segment 0x1000
    for (i = 0; i < 12; i++)
    {
        getblk((u16)ip->i_block[i], 0);
        //prints("d");
        inces();
    }
    //4. load INDIRECT blocks, if any, into memory
    while (*iblk2 != 0)
    {
        getblk((u16)*iblk2, 0);
        //prints("i");
        inces();
        *iblk2++;
    }
    prints("go?"); getc();
}  