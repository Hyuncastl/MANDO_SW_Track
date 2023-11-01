#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() 
{
    int ringBuffer[5] = { 0 };
    int index = 0;
    while (1) 
    {
        int input;
        if (scanf("%d", &input) != 1) 
        {
            printf("입력이 유효하지 않습니다. 프로그램을 종료합니다.\n");
            break;
        }
        ringBuffer[index] = input;
        for (int i = 0; i < 5; i++) 
        {
            printf("%d ", ringBuffer[i]);
        }
        printf("\n");
        index = (index + 1) % 5;
    }
    return 0;
}
