#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int func_a(int month, int day)
{
	int month_list[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int total = 0;
	
	for(int i =1; i<month; i++)
	{
		total += month_list[i-1];
	}
	
	return total -1;
}

int solution (int start_month, int start_day, int end_month, int end_day)
{
	int start_total = func_a(start_month,start_day);
	int end_total = func_a(end_month,end_day);
	
	return end_total - start_total;
}

int main()
{
	int start_month = 1;
	int start_day = 2;
	int end_month = 2;
	int end_day = 2;
	int ret = solution(start_month,start_day,end_month,end_day);
	printf("%d\n",ret);
}
