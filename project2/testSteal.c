#include<stdio.h>
#include<linux/unistd.h> /* Defines _syscall0 and has mygetpid syscall number */
#include<errno.h> /* need this for syscall0 macro too */
//_syscall0(long, mygetpid) /*Can call "mygetpid" now */
/* If you are using 2.6.24, delete the above line */
int main(int argc, char *argv[])
{
	long pid = 0;

	if(argc < 2)
	{
		printf("Not enough args.\n");
//		system.exit(0);
	}
	else
	{
		pid = atol(argv[1]);
//printf("Process ID: %d\n", mygetpid());
		printf("Process ID: %d\n", pid);
		printf("success? %d\n", syscall(326, pid));
//pause();
/* For 2.6.24, change to "printf("Process ID: %d\n", syscall(325)); */
	}
return 0;
}
