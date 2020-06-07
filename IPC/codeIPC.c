#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


#define MAX_NUM 5
int fd[2];
int flag = 1;



void catch(int signum) 
{
   signal(SIGUSR1, catch);
}


void stop(int signum)
{
   signal(SIGUSR2, stop);
   flag = 0;
}


void child() 
{
    signal(SIGUSR1, catch);
    signal(SIGUSR2, stop);
    int i = 0;
    write(fd[1], &i, sizeof(int));
    pid_t ppid = getppid();
    kill(ppid, SIGUSR1);
    pause();
    while (flag)
    {
	//printf("child");
        int i;
        read(fd[0], &i, sizeof(int));
        printf("%d\n",i);

        if (i >= MAX_NUM)
	{
            sleep(1);
            kill(ppid, SIGUSR2);
            return;
        }
        i++;
        write(fd[1], &i, sizeof(int));
        sleep(1);
        kill(ppid, SIGUSR1);
        pause();
    }
}

void parent(pid_t childpid) 
{
    signal(SIGUSR1, catch);
    signal(SIGUSR2, stop);
    pause();

    while (flag)
	{
	//printf("parent");
        int i;
        read(fd[0], &i, sizeof(int));
        printf("%d\n",i);
        if (i >= MAX_NUM)
	{
            sleep(1);
            kill(childpid, SIGUSR2);
            return;
        }
        i++;
        write(fd[1], &i, sizeof(int));
        sleep(1);
        kill(childpid, SIGUSR1);
        pause();
    }
}

int main() 
{
    signal(SIGUSR1, catch);
    signal(SIGUSR2, stop);
    pipe(fd);
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Fork error\n");
        return -1;
    }
    if (pid == 0)
    {
        child();
    }
    else
    {
        parent(pid);
    }

   if(pid == 0)
   {
      printf("Child is going to be terminated\n");
      
   }
   else
   {
      printf("Parent is going to be terminated\n"); 
   }
   

   close(fd[0]);
   close(fd[1]);
   usleep(1000000); //1sec
   return 0;
}
