/***************** queue.c file *****************/
int enqueue(PROC **queue, PROC *p) 
{
   /* enter p into queue by priority*/
   PROC *q = *queue;
   if (q == 0){
      *queue = p;
      p->next = q;
   }
   else if (p->priority > q->priority)
   {
      p->next = q;
      q = p;
   }
   else{
      while (q->next && p->priority <= q->next->priority)
               q = q->next;
      p->next = q->next;
      q->next = p;
   }
}

PROC *dequeue(PROC **queue) 
{
   /* remove & return first PROC from queue*/
   PROC *p = *queue;
   if (p)
      *queue = (*queue)->next;
   return p;
}

int printList(char *name, PROC *list) 
{
   /* print list elements*/
   printf("%s = ", name);
   while(list){
      printf("[%d %d]->", list->pid, list->priority);
      list = list->next;
   }
   printf("NULL\n\n");
}