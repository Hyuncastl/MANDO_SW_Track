#include <stdio.h>

int Jarki_DFS(int x, int y, int count)
{
	static int cnt=0;
	if(count>=x)
		return cnt;
	cnt++;
	if(count<y)
		return Jarki_DFS(x,y,count*2);
	else
		return Jarki_DFS(x,y,count+y);
}

int main()
{
	int a,b;
	
	scanf("%d %d",&a,&b);
	printf("%d\n",Jarki_DFS(a,b,1));
	
	return 0;
}
