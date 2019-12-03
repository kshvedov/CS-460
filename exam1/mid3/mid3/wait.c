int ksleep(int event)
{
  int sr = int_off();
    printf("proc %d going to sleep on event=%x\n", running->pid, event);
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);       //this
    proc[running->ppid].status = READY; //this
    tswitch();
  int_on(sr);
}

int kwakeup(int event)
{
  //printf("PROC EVENT %d woken up\n", event);
  int sr = int_off();
  PROC *p, *t = 0;
    /*for (int i = 1; i < NPROC; i++)
    {
      if(proc[i].status == SLEEP && proc[i].event == event)
      {
        printf("proc %d wakeup %d\n", running->pid, proc[i].pid);
        //proc[i].event = 0;
        proc[i].status == READY;
        //printf("6\n");
        //dequeue(&proc[i]);
        //printf("7\n");
        //printList("\nBefore ready list", readyQueue);
        enqueue(&readyQueue, &proc[i]);
        //printList("After ready list", readyQueue);
        //printf("\n");
        //printf("8\n");
      }
    }*/
  while((p = dequeue(&sleepList))!=0)
  {
    if (p->event==event)
    {
      p->status = READY;
      enqueue(&readyQueue, p);
      printf("proc %d waking up proc %d", running->pid, p->pid);
    }
    else
    {
      enqueue(&t, p);
    }
  }
  sleepList = t;
  int_on(sr);
}

      
int kexit(int exitCode)
{
  //P1 should never die!
  if (running->pid == 1) return -1;
  sendChild();  // give children to P1

  running->exitCode = exitCode;
  running->status = ZOMBIE;
 
  kwakeup((int)running->parent);
  tswitch();
}

int sendChild()
{
  PROC *p; int i;
  for (i = 2; i < NPROC; i++)
  {
    p = &proc[i];
    if((p->status != FREE) && (p->ppid == running->pid))
    {
      if(p->status == ZOMBIE)
      {
        p->status = FREE;
        enqueue(&freeList, p);
      }
      else
      {
        printf("send child %d to P1\n", p->pid);
        p->ppid = 1;
        p->parent = &proc[1];
      }
    }
  }
}

PROC *findZombieChild(PROC *p)
{
  PROC *q = p->sibling;
  while(p->status != ZOMBIE)
  {
    if (q == 0)
      return 0;
    else
    {
      p = q;
      q = q->sibling;
    }
  }
  return p;
}

int kwait(int *status)
{
  int i;
  PROC *p;

  p = running;

  if (p->child == 0){
    printf("wait error: no child\n");
    return -1;
  }
  p = p->child;
  while(1){
    p = findZombieChild(p);   //finds ZOMBIE or returns null
    if (p != 0 && p->status == ZOMBIE && p->ppid == running->pid)
    {
      i = p->pid;             //get ZOMBIE child PID
      status = &p->exitCode;   //copy ZOMBIE child exitCode to *status
      p->status = FREE;       //release child PROC to freeList as FREE
      enqueue(&freeList, p);
      return i;               //return ZOMBIE child pid
    }
    ksleep((int)running);
  }
}


