int enqueue(PROC **queue, PROC *p)
{
  //printf("9\n");
  /*printf("Trying to add proc %d\n", p->pid);
  PROC *q  = *queue;
  //printf("10\n");
  if (q==0 || p->priority > q->priority){
    *queue = p;
    p->next = q;
    return;
  }
  //printf("11\n");
  while (q->next && p->priority <= q->next->priority){
    if (q->pid != q->next->pid)
      q = q->next;
    else
      q->next = 0;
  }
  //printf("12\n");

  printf("p=%d q=%d q->next=%d\n", p->pid, q->pid, q->next->pid);
  if (p->pid == q->pid)
  {
    printf("Cant add proc, already exists!\n");
    return 0;
  }
  p->next = q->next;
  q->next = p;*/
  //printf("13\n");
  PROC *q  = *queue;
  if (q==0 || p->priority > q->priority){
    *queue = p;
    p->next = q;
    return;
  }
  while (q->next && p->priority <= q->next->priority){
    q = q->next;
  }
  p->next = q->next;
  q->next = p;
}

PROC *dequeue(PROC **queue)
{
  PROC *p = *queue;
  if (p)
    *queue = p->next;
  return p;
}

int printList(char *name, PROC *list)
{
  printf("%s: ", name);
  PROC *p = list;
  while(p){
    printf("[%d%d]->", p->pid, p->priority);
    p = p->next;
  }
  printf("NULL\n");
}
