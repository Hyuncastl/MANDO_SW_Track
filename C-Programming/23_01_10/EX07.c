#include <stdio.h>

int main()
{
	int N,K;
	scanf("%d %d",&N,&K);
	if((N>=2&&N<=100000)&&(K>=2&&K<=1000))
	{
		N-=K*(K+1)/2;
		if(N<0)
			printf("-1\n");
		else
			printf("%d\n",K-1+(N%K!=0));
	}
	return 0;
}
