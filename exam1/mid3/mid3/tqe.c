#define NPROC 9

typedef struct tq{
    struct tq *next;        // next element pointer
    int time;               // requested time
    PROC *proc;      // pointer to PROC
}TQE;

TQE *tq, tqe[NPROC];        // tq = timer queue pointer

int tqe_init()
{
    int i; 
    TQE *t;
    kprintf("TQE_init()\n");
    for (i=0; i<NPROC; i++){
        t = &tqe[i];
        t->next = 0;
        t->time = 0;
        t->proc = 0;
    }
    tq = 0;
}

int enqueueTQE(TQE *new_tqe)
{
    printTQEqueue();
    if(tq == 0)
    {
        tq = new_tqe;
    }
    else
    {
        TQE *t = tq;
        while(t->next != 0 && t->next->time < new_tqe->time)
        {
            new_tqe->time -= t->time;
            t = t->next;
        }
        new_tqe->time -= t->time;
        new_tqe->next = t->next;
        t->next = new_tqe;
        if (new_tqe->next != 0)
            new_tqe->next->time -= new_tqe->time;
    }
    printTQEqueue();
}

int procRunner(int pid)
{
    printf("Running proc %d\n", pid);
    while(1)
    {
        //running
    }
}

int printTQEqueue()
{
    TQE *t = tq;
    if (t == 0)
    {
        startT = 0;
        printf("enter command : ");
        return 0;
    }
    printf("tq --> ");
    while (t != 0)
    {
        printf("tqe[%d, *%d] --> ", t->time, t->proc->pid);
        t = t->next;
    }
    printf("NULL\n");
}

int decTQE()
{
    TQE* t  = tq;
    if (t->time != 0)
    {
        t->time--;
    }
    else
    {
        PROC *p = t->proc;
        kwakeup(p->event);
        printf("proc %d woke up in TQE\n", p->pid);
        tq = t->next;
        t->next = 0;
        t->time = 0;
        t->proc = 0;
    }
    
}

PROC *kfork(int func, int priority);

int new_tqeProc(int time)
{
    printf("New TQE with time %d\n", time);
    int i = 0;
    TQE *t = &tqe[i];
    while(t->proc != 0)
    {
        i++;
        if (i >= NPROC)
        {
            printf("No TQE's left!\n");
            return 0;
        }
        t = &tqe[i];
    }
    t->time = time;
    t->proc = kfork((int)procRunner, 1);
    enqueueTQE(t);
}