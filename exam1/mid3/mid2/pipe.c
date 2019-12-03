typedef struct pipe{
    char    buf[PSIZE];
    int     head, tail;
    int     data, room;
    int     status;
}PIPE;

PIPE pipe[NPIPE];

int pipe_init()
{
    int i, j = 0;
    printf("pipe_init()\n");
    printf("creating pipe ");
    for (i = 0; i < NPIPE; i++)
    {
        pipe[i].head = pipe[i].tail = pipe[i].data = 0;
        pipe[i].room = PSIZE;
        pipe[i].status = FREE;
        if (pipe[i].status == FREE)
        {
            printf(".");
            j++;
        }
    }
    if (j == 4)
        printf("OK\n");
}

PIPE *create_pipe()
{
    PIPE *p;
    int i = 0;

    while(i < NPIPE)
    {
        p = &pipe[i++];
        if(p->status == FREE)
        {
            p->status = BUSY;
            return p;
        }
    }
    printf("NO PIPES LEFT!\n");
    return 0;
}

/*---------- Algorithm of pipe_read-------------*/
int read_pipe(PIPE *p, char *buf, int n)
{
    int r = 0;
    if (n<=0)
        return 0;
    //validate PIPE pointer p;
    if (p->status == FREE)
    {
        printf("p->status must not be FREE \n");
        return 0;
    }
    while(n)
    {
        while(p->data){
            *buf++ = p->buf[p->tail++];  // read a byte to buf
            p->tail %= PSIZE;
            p->data--; p->room++; r++; n--;
            if (n==0)
                break;
        }
        kwakeup(&p->room);           // wakeup writers
        //printf("1\n");
        //printf("r = %d\n", r);
        if (r)                       // if has read some data
            return r;
        //printf("2\n");
        // pipe has no data
        ksleep(&p->data);             // sleep for data
    }
}

/*---------- Algorithm of write_pipe -----------*/
int write_pipe(PIPE *p, char *buf, int n)
{
    int r = 0;
    if (n<=0)
        return 0;
    //validate PIPE pointer p;
    if (p->status == FREE)
    {
        printf("p->status must not be FREE \n");
        return 0;
    }
    while(n)
    {
        //printf("pRoom = %d, pData= %d\n", p->room, p->data);
        while(p->room)
        {
            p->buf[p->head++] = *buf++; // write a byte to pipe;
            p->head %= PSIZE;
            p->data++; p->room--; r++; n--;
            if (n==0)
                break;
        }
        printf("proc %d wrote %d bytes\n", running->pid, r);
        kwakeup(&p->data);   // wakeup readers, if any.
        if (n==0)
        {
            //printf("4\n");
            return r;   // finished writing n bytes
        }
        // still has data to write but pipe has no room
        ksleep(&p->room);    // sleep for room
        //printf("3\n");
    }
}

int destroy_pipe(PIPE *p)
{
    //validate PIPE pointer p;
    if (p->status == FREE)
    {
        printf("p->status must not be FREE\n");
        return 0;
    }
    kwakeup(&p->room);           // wakeup writers
    kwakeup(&p->data);           // wakeup readers, if any.
    p->head = p->tail = p->data = 0;
    p->room = PSIZE;
    p->status = FREE;
    return 1;
}