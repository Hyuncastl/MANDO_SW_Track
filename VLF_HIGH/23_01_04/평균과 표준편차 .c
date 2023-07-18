#pragma warning(disable :4996)
#include <stdio.h>
#include <math.h>
#define MAX_DATA_NO 100

double sonar_data[MAX_DATA_NO] = { -1, };
int no_line = -1;

int sonar_dat(){
	double temp;
	int result;
	FILE* fs = fopen("C:/Users/user/Desktop/3/sonardat.txt", "r");
	if (fs == NULL)
	{
		printf("file does not EXIT");
	}
	else
	{
		do
		{
			result = fscanf(fs, "%lf", &temp);
			sonar_data[++no_line] = temp;
		} while (result != EOF);

		printf("line : %d\n", no_line);

		for (int i = 0; i < no_line; i++)
		{
			printf("%2d %3.2lf\n", i + 1, sonar_data[i]);
		}
		fclose(fs);
	}
	return 0;
}

float average_data() {
	int i;
	double Sum;
	double m=0;
	double v;
	double a;

	

	Sum = 0;
	for (i = 0; i < no_line; i++)
	{
		Sum += sonar_data[i];
		m = Sum / no_line;
	}

	Sum = 0;
	for (i = 0; i < no_line; i++) {
		Sum += (sonar_data[i] - m) * (sonar_data[i] - m);
	}
	v = Sum / no_line;

	a = sqrt(v);

	printf("평균 : %3.2lf\n", m);
	printf("표준편차 : %3.2lf\n", a);

	return 0;

}

int main()
{
	sonar_dat();
	average_data();
	return 0;
