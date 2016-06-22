#include"headers.h"

int make_request_fifo();
int make_result_fifo();
int make_prequest_fifo();
int make_presult_fifo();

static int flag[SIZE];

void signalhandler1(int sig)
{
	flag[0]=1;
}

void signalhandler2(int sig)
{
	flag[1]=1;
}

void signalhandler3(int sig)
{
	flag[2]=1;
}

void quit(int sig)
{
	exit (EXIT_SUCCESS);
}

static int set_semaphore_value();
static int p();
static int v();

union semun
{
	int val;
};

static int semid;

int main()
{
	printf("This is server... \n");           
	
	make_request_fifo();
	make_result_fifo();
	make_prequest_fifo();
	make_presult_fifo();

	semid=semget((key_t)123,1,0666|IPC_CREAT);

	if(!set_semaphore_value())
	{
		printf("Semaphore value not initialized... \n");
		exit (EXIT_FAILURE);
	}

	printf("Semaphore value initialized successfully... \n");
	
	int pcpid[SIZE];
	int i;
	char *pclients[SIZE]={"pclient1","pclient2","pclient3"};

	for(i=0;i<SIZE;i++)
	{
		pcpid[i]=fork();

		if(pcpid[i]==0)
		{
			execl(pclients[i],pclients[i],NULL);
		}
	}

	sleep(1);
		
	printf("Pclients invoked... \n");

	int reqopen,resopen,preqopen,presopen;
	
	reqopen=open("./requestfifo",O_RDONLY|O_NONBLOCK);
	resopen=open("./resultfifo",O_WRONLY);
	preqopen=open("./prequestfifo",O_WRONLY);
	presopen=open("./presultfifo",O_RDONLY);
	
	int count=0;
	struct request r,req[3];
	char sign[SIZE]={'+','*','/'};
	char *clients[SIZE]={"Client1","Client2","Client3"};
	
	(void) signal(SIGALRM,signalhandler1);
	(void) signal(SIGHUP,signalhandler2);
	(void) signal(SIGILL,signalhandler3);
	(void) signal(SIGINT,quit);
	int results[SIZE];

	while(1)
	{
		count=read(reqopen,&r,sizeof(struct request));
	
		if(count>0)
		{
			printf("\nGot a request... \n");
			
			for(i=0;i<SIZE;i++)
			{
				if(r.sign==sign[i])
				{
					req[i]=r;
					p();
					kill(pcpid[i],SIGALRM);
					write(preqopen,&req[i],sizeof(struct request));
					break;
				}
			}
		}

		for(i=0;i<SIZE;i++)
		{
			if(flag[i]==1)
			{
				read(presopen,&results[i],4);
//				kill(req[i].cpid,SIGALRM);
//				write(resopen,&results[i],4); 
				flag[i]=0;
			}
		}
	}

	return 0;
OUT:
	return -1;
}

/************************************************************************************************************************************/

int make_request_fifo()
{
	int reqfifofd;

        if(access("./requestfifo",F_OK)==-1)
        {
                reqfifofd=mkfifo("./requestfifo",0777);

                if(reqfifofd==0)
                {
                        printf("requestfifo created successfully... \n");
                }

                else
                {
                        printf("requestfifo creation failed... \n");
                        exit (EXIT_FAILURE);
                }
        }

        else
        {
                printf("requestfifo already created... \n");
        }

	return 0;
}

int make_result_fifo()
{
	int resfifofd;

        if(access("./resultfifo",F_OK)==-1)
        {
		resfifofd=mkfifo("./resultfifo",0777);

                if(resfifofd==0)
                {
                        printf("resultfifo created successfully... \n");
                }

                else
                {
                        printf("resultfifo creation failed... \n");
                        exit (EXIT_FAILURE);
                }
        }

        else
        {
                printf("resultfifo already created... \n");
        }

	return 0;
}

int make_prequest_fifo()
{
	int preqfifofd;

        if(access("./prequestfifo",F_OK)==-1)
        {
                preqfifofd=mkfifo("./prequestfifo",0777);

                if(preqfifofd==0)
                {
                        printf("prequestfifo created successfully... \n");
                }

                else
                {
                        printf("prequestfifo creation failed... \n");
                        exit (EXIT_FAILURE);
                }
        }

        else
        {
                printf("prequestfifo already created... \n");
        }

	return 0;
}

int make_presult_fifo()
{
	int presfifofd;

        if(access("./presultfifo",F_OK)==-1)
        {
                presfifofd=mkfifo("./presultfifo",0777);

                if(presfifofd==0)
                {
                        printf("presultfifo created successfully... \n");
                }

                else
                {
                        printf("presultfifo creation failed... \n");
                        exit (EXIT_FAILURE);
                }
        }

        else
        {
                printf("presultfifo already created... \n");
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
