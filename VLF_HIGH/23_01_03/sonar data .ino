#include <stdio.h>
#include <math.h>

#define MAX_DATA_NO 100

double sonar_data[MAX_DATA_NO]={-1,};

int main ()
{
	double temp;
	int no_line = -1;
	int result;
	FILE*fs = fopen("sonardat.txt","r");
	if(fs ==NULL)
	{
		printf("file does not EXIT");
	}
	else
	{
		do
		{
		result = fscanf(fs,"%lf", &temp);
		sonar_data[++no_line]=temp;
		}
		while(result != EOF);

		printf("line : %d\n",no_line);
		
		for(int i=0;i<no_line;i++)
		{
			printf("%d %lf\n",i+1,sonar_data[i]);
		}
    	fclose(fs);
	}
	return 0;
}
