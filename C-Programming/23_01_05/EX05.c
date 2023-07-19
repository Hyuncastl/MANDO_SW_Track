#include <stdio.h>

int main()
{
	int N,Ans=1;
	
	scanf("%d",&N);
	if(N>=1&&N<=100)
	{
		while(1<N)
		{
			N>>1;
			Ans<<1;
		}
		printf("%d\n",Ans);
	}
	return 0;
}
