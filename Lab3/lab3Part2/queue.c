/***************** queue.c file *****************/
/* enter p into queue by priority*/ 
int enqueue(PROC **queue, PROC *p) 
{
   PROC *q = *queue;
   if (q == 0 || p->priority > q->priority){
      *queue = p;
      p->next = q;
   }
   else{
      while (q->next && p->priority <= q->next->priority)
               q = q->next;
      p->next = q->next;
      q->next = p;
   }
}

/* remove & return first PROC from queue*/ 
PROC *dequeue(PROC **queue) 
{
     PROC *p = *queue;
     if (p)
        *queue = (*queue)->next;
     return p;
}

/* print list elements*/
int printList(char *name, PROC *list) 
{
  printf("%s = ", name);
  while(list){
     printf("[%d %d]->", list->pid, list->priority);
     list = list->next;
  }
  printf("NULL\n");
}