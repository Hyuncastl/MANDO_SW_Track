#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

int solution(int calorie[], int calorie_len)
{
	int answer = 0;
	int min = calorie[0];

	for (int i = 0; i < calorie_len; i++)
	{
		if (calorie[i] > min)
			answer += calorie[i] - min;
		else
			min = calorie[i];
	}

	return answer;
}

int main()
{
	int calorie[] = { 713,665,873,500,751 };
	int ret = solution(calorie, 5);
	printf("solution 함수의 반환 값은 %d입니다.\n", ret);
}
