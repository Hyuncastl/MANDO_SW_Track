#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


int main() {
    int ringBuffer[5] = { 0 }; // 5개의 요소를 가진 배열을 초기화합니다.
    int index = 0;

    while (1) {
        int input;
        if (scanf("%d", &input) != 1) {
            printf("입력이 유효하지 않습니다. 프로그램을 종료합니다.\n");
            break;
        }

        ringBuffer[index] = input;

        for (int i = 0; i < 5; i++) {
            printf("%d ", ringBuffer[i]);
        }
        printf("\n");

        index = (index + 1) % 5; // 인덱스를 업데이트하여 ring buffer의 순환을 유지합니다.
    }

    return 0;
}
