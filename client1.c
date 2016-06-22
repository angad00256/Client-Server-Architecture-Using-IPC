#include"headers.h"

static int flag=0;

void signalhandler(int sig)
{
	flag=1;
	printf("Client1 result achieved... \n");
}

void quit(int sig)
{
	exit (EXIT_SUCCESS);
}

int main()
{
	(void) signal(SIGALRM,signalhandler);
	(void) signal(SIGINT,quit);
	int reqopen;
	struct request req;
	int resopen;
	int sum;
	
	printf("This is Client1... \n");

	reqopen=open("./requestfifo",O_WRONLY);

	if(reqopen<0)
	{
		perror("open");
		goto OUT;
	}

	resopen=open("./resultfifo",O_RDONLY);

	if(resopen<0)
	{
		perror("open");
		goto OUT;
	}

	req.op1=12;
	req.op2=14;
	req.sign='+';
	req.cpid=getpid();

	printf("Client1 pid : %d \n",req.cpid);

	write(reqopen,&req,sizeof(struct request));

	pause();

	if(flag==1)
	{
		read(resopen,&sum,4);
		printf("Sum = %d \n",sum);
	}

	return 0;
OUT:
	return -1;
}

