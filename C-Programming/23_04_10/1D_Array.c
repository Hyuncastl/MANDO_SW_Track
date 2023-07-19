// int*로 받고, 인덱스 접근

#include<stdio.h>
int* getNumbers() 
{
    static int arr[6] = {1,2,3,4,5,6};
    return arr;
}
int main() 
{
    int* nums = getNumbers();
    for(int i=0; i<6; i++)
    {
		printf("%d ", nums[i]);
    }
	printf("\n");
}
