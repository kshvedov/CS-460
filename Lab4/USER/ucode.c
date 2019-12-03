int umenu()
{
  uprintf("-----------------------------------------------\n");
  uprintf("getpid getppid ps chname switch kfork exit wait\n");
  uprintf("-----------------------------------------------\n");
}

int getpid()
{
  int pid;
  pid = syscall(0,0,0,0);
  return pid;
}    

int getppid()
{ 
  return syscall(1,0,0,0);
}

int ugetpid()
{
  int pid = getpid();
  uprintf("pid = %d\n", pid);
}

int ugetppid()
{
  int ppid = getppid();
  uprintf("ppid = %d\n", ppid);
}

int ups()
{
  return syscall(2,0,0,0);
}

int uchname()
{
  char s[32];
  uprintf("input a name string : ");
  ugetline(s);
  printf("\n");
  return syscall(3,s,0,0);
}

int uswitch()
{
  return syscall(4,0,0,0);
}

int ugetc()
{
  return syscall(90,0,0,0);
}

int uputc(char c)
{
  return syscall(91,c,0,0);
}

int getPA()
{
  return syscall(92,0,0,0);
}

int ukfork()
{
  char s[128];
  int pid;
  printf("What file do you want to fork?\n");
  ugetline(s);
  pid = syscall(5,s,0,0);
  return pid;
}

int uexit()
{
  char s[128];
  int i;
  printf("Exit with value: ");
  ugetline(s);
  i = atoi(s);
  return syscall(6,i,0,0);
}

int uwait()
{
  int pid, status;
  pid = syscall(7,&status,0,0);
  printf("Child pid=%d   Status=%d", pid, status);
  return;
}