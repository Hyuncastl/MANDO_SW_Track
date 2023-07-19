#include <stdio.h>
#include <stdlib.h>

int main()
{
	int N;
	int i,j;
	long long int Sum;
	int Pn[4]={1,0,0,0};
	long long int Psum=0;
	scanf("%d",&N);
	if(N>=4&&N<=100000)
	{
		int *M=(int *)malloc(sizeof(int)*N);
		for(i=0;i<N;i++)
		{
			scanf("%d",&M[i]);
			if(M[i]<-1000||M[i]>1000)
				return 0;
		}
		Sum=0;
		for(i=0;i<N;i++)
		{
			Sum+=(long long int)M[i];
		}
		for(j=0;j<N;j++)
		{
			Psum+=(long long int)M[j];
			for(i=3;i>=0;i--)
				if(Psum==Sum*i/4)
					Pn[i]+=Pn[i-1];
		}
		printf("%d",Pn[3]);
		free(M);
	}
	return 0;
}
