#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int	main()
{
	
	int open1 = open("txt.txt", O_WRONLY | O_CREAT);
	printf ("open1 fd = %d\n", open1);
	close(1);
	open1 = dup(open1);
	printf("open1 after close 2 and dup him self = %d\n", open1);
	execve("/usr/bin/ls",(char *[]) {"ls", NULL}, NULL);
	return (0);
}
