#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 이항 계수 계산 함수
unsigned long long binomialCoefficient(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    }
    else {
        unsigned long long result = 1;
        for (int i = 1; i <= k; i++) {
            result *= (n - i + 1);
            result /= i;
        }
        return result;
    }
}

// (x + a)^n을 전개하고 출력하는 함수
void expandBinomial(int n) {
    printf("(%c + a)^%d = ", 'x', n);

    for (int k = 0; k <= n; k++) {
        unsigned long long coefficient = binomialCoefficient(n, k);
        printf("%llu * %c^%d * a^%d", coefficient, 'x', n - k, k);

        if (k != n) {
            printf(" + ");
        }
        else {
            printf("\n");
        }
    }
}

int main() {
    int n;
    printf("차수를 입력하세요: ");
    scanf("%d", &n);

    expandBinomial(n);

    return 0;
}
