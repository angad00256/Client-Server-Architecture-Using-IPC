#include"headers.h"

static int flag=0;

void signalhandler(int sig)
{
	flag=1;
	printf("Client2 result achieved... \n");
}

void quit(int sig)
{
	exit (EXIT_SUCCESS);
}

int main()
{
	(void) signal(SIGHUP,signalhandler);
	(void) signal(SIGINT,quit);
	int reqopen;
	struct request req;
	int resopen;
	int product;
	
	printf("This is Client2... \n");

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

	req.op1=5;
	req.op2=7;
	req.sign='*';
	req.cpid=getpid();

	printf("Client2 pid : %d \n",req.cpid);

	write(reqopen,&req,sizeof(struct request));

	pause();

	if(flag==1)
	{
		read(resopen,&product,4);
		printf("Product = %d \n",product);
	}

	return 0;
OUT:
	return -1;
}

