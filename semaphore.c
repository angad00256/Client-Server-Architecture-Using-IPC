#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

static int set_semaphore_value();
static void delete_semaphore();
static int p();
static int v();

union semun
{
	int val;
};

static int semid;

int main()
{
	int cpid;
	int i;
	int n=5;
	int status;

	semid=semget((key_t)123,1,0666|IPC_CREAT);

	if(!set_semaphore_value())
	{
		printf("Semaphore value not initialized... \n");
		exit (EXIT_FAILURE);
	}

	printf("Semaphore value initialized successfully... \n");

	cpid=fork();

	switch(cpid)
	{
		case 0: n=2;
			p();
			for(i=0;i<=n;i++)
			{
				printf("This is child... \n");
				sleep(2);
			}
			v();
			break;

		default: p();
			 for(i=0;i<=n;i++)
			 {
			 	printf("This is parent... \n");
				sleep(2);
			 }
			 v();
			 break;
	}

	wait(&status);

	if(cpid!=0)
	{
		delete_semaphore();
	}

	return 0;
}

static int set_semaphore_value()
{
	union semun sem_un;

	sem_un.val=1;

	if(semctl(semid,0,SETVAL,sem_un)==-1)
	{
		return 0;
	}

	return 1;
}

static void delete_semaphore()
{
	union semun sem_un;

	if(semctl(semid,0,IPC_RMID,sem_un)==-1)
	{
		printf("Semaphore not deleted... \n");
		exit (EXIT_FAILURE);
	}

	else
	{
		printf("Semaphore deleted successfully... \n");
	}
}

static int p()
{
	struct sembuf sem_buf;

	sem_buf.sem_num=0;
	sem_buf.sem_op=-1;
	sem_buf.sem_flg=SEM_UNDO;

	if(semop(semid,&sem_buf,1)==-1)
	{
		printf("Semaphore p function failed... \n");
		return 0;
	}

	return 1;
}
	
static int v()
{
	struct sembuf sem_buf;

	sem_buf.sem_num=0;
	sem_buf.sem_op=1;
	sem_buf.sem_flg=SEM_UNDO;

	if(semop(semid,&sem_buf,1)==-1)
	{
		printf("Semaphore v function failed... \n");
		return 0;
	}

	return 1;
}	
