#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <pthread.h>

void *str_rev(void *sir)
{
	char *str=(char *)sir;
	char *sirrev=malloc(strlen(str)*sizeof(char));
	for(int i=0;i<=strlen(str)-1;i++)
	{
		sirrev[i]=str[strlen(str)-1-i];
	}
	strcat(sirrev,"\0");
	return sirrev;
} 

int main(int argc,char *argv[])
{
	pthread_t thr;
	char *rezultat;
	if(pthread_create(&thr,NULL,str_rev,argv[1]))
	{
		perror("pthread_create");
		return errno;
	}
	if(pthread_join(thr,(void **)&rezultat))
	{
		perror("pthread_join");
		return errno;
	}
	printf("%s\n",rezultat);
	return 0;
}
