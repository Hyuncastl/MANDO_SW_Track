#include <stdio.h>
#include <stdlib.h>

int main()
{
	int T,i,ADDH,ADDT,MINT,ADDO,MINO;
	scanf("%d",&T);
	if(T>=1 && T<=100)
	{
		int *N=(int*)malloc(sizeof(int)*T);
		for(i=0;i<T;i++)
		{
			scanf("%d",&N[i]);
			if(N[i]<1 || N[i]>10000000)
				return 0;
		}
		for(i=0;i<T;i++)
		{
			ADDH = 0; ADDT = 0; MINT = 0; ADDO = 0; MINO = 0;
			ADDH += 60;
			N[i]%=60;
			if(N[i]>35)
			{
				ADDH++;
				MINT=6-(N[i]+5)/10;
				N[i]%=10;
				if(N[i]>=5)
					MINO+=10-N[i];
				else
					ADDO+=N[i];
			}
			else
			{
				ADDT=(N[i]+4)/10;
				N[i]%=10;
				if(N[i]>=6)
				{
					MINO+=10-N[i];
				}
				else 
				{
					ADDO+=N[i];
				}
			}
			printf("%d %d %d %d %d\n",ADDH,ADDT,MINT,ADDO,MINO);
		}
		free(N);
	}
	return 0;
}
