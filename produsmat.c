#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

struct lincol{int dim;int *v;int *u;};

void *prodmat(void *t)
{
	struct lincol *s=(struct lincol *)t;
	int *sum=malloc(sizeof(int *));
	*sum = 0;
	for(int i=0;i<(*s).dim;i++)
	{
		*sum = *sum+(*s).v[i]*(*s).u[i];
	}
	free(s);
	return sum;
}

int main(int argc,char *argv[])
{
	FILE *fileA,*fileB;
	fileA=fopen(argv[1],"r");
	fileB=fopen(argv[2],"r");
	int l,c,k,i,j;
	fscanf(fileA,"%d",&l);
	fscanf(fileA,"%d",&k);
	fscanf(fileB,"%d",&c);
	int A[l][k],B[k][c];
	for(i=0;i<l;i++)
		for(j=0;j<k;j++)
			fscanf(fileA,"%d",&A[i][j]);
	for(i=0;i<k;i++)
		for(j=0;j<c;j++)
			fscanf(fileB,"%d",&B[i][j]);
	fclose(fileA);
	fclose(fileB);
	pthread_t thr[l*c];
	int p=0;

	struct lincol *x;
	for(i=0;i<l;i++)
		for(j=0;j<c;j++)
		{
			x=malloc(sizeof(struct lincol *));
			(*x).v=malloc(k*sizeof(int));
			(*x).u=malloc(k*sizeof(int));
			(*x).dim=k;
			for(int z=0;z<k;z++)
			{
				(*x).v[z] = A[i][z];
				(*x).u[z] = B[z][j];
			}
			if(pthread_create(&thr[p++],NULL,prodmat,x))
			{
				perror("pthread_create");
				return errno;
			}
		}
	int *elem;
	p=0;
	for(i=0;i<l;i++)
	{
		for(j=0;j<c;j++)
		{
			if(pthread_join(thr[p++],(void **)&elem))
			{
				perror("pthread_join");
				return errno;
			}
			printf("%d ",*elem);
		}
		printf("\n");
	}
	return 0;
}
