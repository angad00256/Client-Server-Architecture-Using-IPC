#include"headers.h"

static int flag=0;

void signalhandler(int sig)
{
	flag=1;
	printf("Client3 result achieved... \n");
}

void quit(int sig)
{
	exit (EXIT_SUCCESS);
}

int main()
{
	(void) signal(SIGILL,signalhandler);
	(void) signal(SIGINT,quit);
	int reqopen;
	struct request req;
	int resopen;
	int quotient;
	
	printf("This is Client3... \n");

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

	req.op1=45;
	req.op2=5;
	req.sign='/';
	req.cpid=getpid();

	printf("Client3 pid : %d \n",req.cpid);

	write(reqopen,&req,sizeof(struct request));

	pause();

	if(flag==1)
	{
		read(resopen,&quotient,4);
		printf("Quotient = %d \n",quotient);
	}

	return 0;
OUT:
	return -1;
}

