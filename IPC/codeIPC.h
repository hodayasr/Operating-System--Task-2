#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void catch_(int signum);
void stop(int signum);
void child();
void parent(pid_t childpid);
