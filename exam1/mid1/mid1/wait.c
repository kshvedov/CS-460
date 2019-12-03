int ksleep(int event)
{
  int sr = int_off();
    printf("proc %d going to sleep on event=%x\n", running->pid, event);
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);
    proc[running->ppid].status = READY;
    tswitch();
  int_on(sr);
}

int kwakeup(int event)
{
  PROC *p, *temp=0;
  int sr = int_off();
    /*for (int i = 0; i < NPROC; i++)
    {
      if(proc[i].status == SLEEP && proc[i].event == event)
      {
        proc[i].status == READY;
        enqueue(&readyQueue, &proc[i]);
      }
      //printf("\nThere may be an issue in kwakeup when running\n");
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
        enqueue(&temp, p);
      }
    }
    sleepList = temp;

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


