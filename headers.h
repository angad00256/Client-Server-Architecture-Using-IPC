#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#define SIZE 3

struct request
{
	int op1;
	int op2;
	char sign;
	pid_t cpid;
};


