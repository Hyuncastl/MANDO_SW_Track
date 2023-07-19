#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
int* solution(int score[], int score_len) {
	int* answer;
	answer = (int*)malloc(sizeof(int) * score_len);
	for (int i = 0; i < score_len; i++) {
		answer[i] = 1;
		for (int j = 0; j < score_len; j++) {
			if (score[i] < score[j])answer[i]++;
			}
		}
	return answer;
}

int main() {
	int score1[] = { 90,87,87,23,35,28,12,46};
	int score_len1 = 8;
	int* arr1 = solution(score1, score_len1);

	printf("값:");
	for (int i = 0; i < score_len1; i++) {
		if (i != 0)
			printf(",");
		printf("%d", arr1[i]);
	}
	int score2[] = { 10,20,20,30 };
	int score_len2 = 4;
	int* arr2 = solution(score2, score_len2);

	printf("값:");
	for (int i = 0; i < score_len2; i++) {
		if (i != 0)
			printf(",");
		printf("%d", arr2[i]);
	}
}
