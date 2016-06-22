#include"headers.h"

static int v();

int flag=0;

void signalhandler(int sig)
{
	flag=1;
}

static int semid;

int main()
{
	int preqopen,presopen;
	(void) signal(SIGALRM,signalhandler);
	struct request req;
	int quotient;

	preqopen=open("./prequestfifo",O_RDONLY);
	presopen=open("./presultfifo",O_WRONLY);

	while(1)
	{
		if(flag==1)
		{	
			read(preqopen,&req,sizeof(struct request));
			
			v();

			printf("Client3 request is : %d %c %d \n",req.op1,req.sign,req.op2);

			quotient=(req.op1)/(req.op2);
		
			printf("Quotient = %d \n",quotient);

//			kill(getppid(),SIGILL);

//			write(presopen,&quotient,4);
		
			flag=0;
		}
	}

	return 0;
OUT:
	return -1;
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
