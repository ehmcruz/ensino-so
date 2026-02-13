#include <stdio.h>
#include <unistd.h>

int main ()
{
	pid_t pid;

	pid = fork();

	if (pid == 0) 
		printf("nooooooooooooooooo\n");
	else 
		printf("I am your father, Luke\n");

	return 0;
}