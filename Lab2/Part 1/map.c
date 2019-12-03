#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
int fd, i, BLK = 4096;
main(int argc, char *argv[])
{
    if((fd = open(argv[1], O_RDONLY)) < 0)
    {   printf("open %s failed\n", argv[1]); exit(1);}
    /*mmap(startAddr, nbytes, usage flags, fileDes, offset)*/
    char *p = mmap(0, BLK, PROT_READ, MAP_SHARED, fd, 0);
    printf("-------- file contents --------\n");
    printf("%s\n", p);
    for (i=0; i<BLK; i++) putchar(*p++);
}