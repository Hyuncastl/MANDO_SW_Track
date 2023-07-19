#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int T;
	char str[255];
	int r,c;
	int c1,c2;
	int chk1,chk2,chk3;
	int i;
	int N;
	
	scanf("%d%*c",&T);
	
	if(T>=1&&T<=30)
	{
		char**M=(char**)malloc(sizeof(char*)*T);
		for(r=0;r<T;r++)
		{
			scanf("%s%*c",str);
			if(strlen(str)>=3&&strlen(str)<=100000)
			{
				M[r]=(char*)malloc(sizeof(char)*(strlen(str)+1));
				stcpy(M[r],str);
			}
			else
				return 0;
		}
		for(r=0;r<T;r++)
		{
			chk1=-1;
			N=strlen(M[r]);
			for(c=0;c<N;c++)
				if(M[r][c]!=M[r][N-1-c])
				{
					chk1=c;
					break;
				}
			if(chk1 == -1)
			{
				printf("0\n");
			}
			else
			{
				c1=c2=0;
				chk2=0;chk3=0;
				char*chkStr1=(char*)malloc(sizeof(char)*(strlen(M[r])+1));
				char*chkStr2=(char*)malloc(sizeof(char)*(strlen(M[r])+1));
				
				for(i=0;i<N;i++)
				{
					if(i!=chk1)chkStr1[c1++]=M[r][i];
					if(i!=N-1-chk1)chkStr2[c2++]=M[r][i];
				}
				for(i=0;i<N;i++)
				{
					if(chkStr1[i]!=chkStr1[N-2-i])chk2=1;
					if(chkStr2[i]!=chkStr2[N-2-i])chk3=1;
				}
				if(chk2==1&&chk3==1)
					printf("2\n");
				else
					printf("1\n");
				
				free(chkStr1);
				free(chkStr2);
			}
		}
		for(r=0;r<T;r++)
			free(M[r]);
		free(M);
	}
	return 0;
}
