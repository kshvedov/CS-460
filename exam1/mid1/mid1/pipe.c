#define PSIZE 8
#define NPIPE 4

typedef struct pipe{
    char    buf[PSIZE];
    int     head, tail;
    int     data, room;
    int     status;
}PIPE;

PIPE pipe[NPIPE];

/*---------- Algorithm of pipe_read-------------*/
int read_pipe(PIPE *p, char *buf, int n)
{
    int r = 0;
    if (n<=0)
        return 0;
    //validate PIPE pointer p;
    //p->status must not be FREE

    while(n)
    {
        while(p->data){
            *buf++ = p->buf[p->tail++];  // read a byte to buf
            p->tail %= PSIZE;
            p->data--; p->room++; r++; n--;
            if (n==0)
                break;
        }
        wakeup(&p->room);            // wakeup writers
        if (r)                       // if has read some data
            return r;
        // pipe has no data
        sleep(&p->data);             // sleep for data
    }
}

/*---------- Algorithm of write_pipe -----------*/
int write_pipe(PIPE *p, char *buf, int n)
{
    int r = 0;
    if (n<=0)
        return 0;
    //validate PIPE pointer p;
    // p->status must not be FREE
    while(n)
    {
        while(p->room)
        {
            p->buf[p->head++] = *buf++; // write a byte to pipe;
            p->head %= PSIZE;
            p->data++; p->room--; r++; n--;
            if (n==0)
                break;
        }
        wakeup(&p->data);   // wakeup readers, if any.
        if (n==0)
            return r;   // finished writing n bytes
        // still has data to write but pipe has no room
        sleep(&p->room);    // sleep for room
    }
}