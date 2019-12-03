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
  //printf("PROC EVENT %d woken up\n", event);
  int sr = int_off();
  PROC *p, *t = 0;
    
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
  int i;
  //P1 should never die!
  if (running->pid == 1) return -1;

  for (i=2; i<NPROC; i++)
  {
    if (proc[i].ppid == running->pid)
    {
      proc[i].ppid = proc[1].pid;
    }
  }

  running->exitCode = exitCode;
  running->status = ZOMBIE;
 
  kwakeup((int)running->ppid);
  tswitch();
}

int kwait(int *status)
{
  int i;
  PROC *p;

  p = running;

  for (i = 0; i < NPROC; i++) // check for children
  {
    if (proc[i].ppid == running->pid && proc[i].status != FREE && proc[i].status != ZOMBIE)
      break;
  }

  if (i == NPROC){
    printf("wait error: no child\n");
    return -1;
  }

  while(1){
    for (i = 0; i < NPROC; i++)
    {
      if (proc[i].status == ZOMBIE && proc[i].ppid == running->pid)
      {
        *status = proc[i].exitCode;   //copy ZOMBIE child exitCode to *status
        proc[i].status = FREE;       //release child PROC to freeList as FREE
        proc[i].priority = 0;
        enqueue(&freeList, &proc[i]);
        return proc[i].pid;               //return ZOMBIE child pid
      }
    }
    ksleep((int)running->pid);
  }
}


