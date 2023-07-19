//void*로 받고, 인덱스 접근

#include<stdio.h>
void* getNumbers2() 
{
    static int arr[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    return (void*) arr;
}
int main() 
{
    int (*nums)[3] = getNumbers2();
    for(int i=0; i<3; i++) 
    {
        for(int j=0; j<3; j++) printf("%d ", nums[i][j]);
        printf("\n");
    }
}
