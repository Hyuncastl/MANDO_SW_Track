#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>


int solution(int price, char*grade)
{
	int answer = 0;
	double percent;
	int sale;
	
	sale = price * percent;
	answer = price - percent;
	
	if(grade == 'S')
	{
		percent = 0.05;
		sale = price * percent;
	answer = price - sale;
	}
	else if (grade == 'G')
	{
		percent = 0.1;
		sale = price * percent;
	answer = price - sale;
	}
	else
	{
		percent = 0.15;
		sale = price * percent;
	answer = price - sale;
	}

	
	return answer;
}

int main ()
{
	char grade;
	int price1 = 2500;
	int price2 = 96900;
	
	char grade1 = 'S';//,"G","V";
	char grade2 = 'G';
	char grade3 = 'V';
	
	int result1 = solution(price1, grade3);
	printf("할인 된 가격: %d\n",result1);
	
	int result2 = solution(price2, grade1);
	printf("할인 된 가격: %d\n",result2);
	
	
	return 0;
	
}
