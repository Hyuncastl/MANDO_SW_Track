#include <stdio.h>
#define CRT_SECURE_NO_WARNINGS

int main()
{
	FILE *fp;
	int i;
	int data1 = 0;
	double data2 = 0.0;
	char ch;
	
	fp=fopen("data3.txt","rt");
	
	if(fp==NULL)
	{
		//printf("File writing Error!\n");
		puts("File reading Error!\n");
		return -1;
	}
	else
	{
		printf("File reading Success!\n");
	}
	
	for(i=0;i<2;i++)
	{
		//ch=fgetc(fp);
		fscanf(fp,"%d",&data1);
		printf("%d\n",data1);
	}
	for(i=0;i<2;i++)
	{
		//ch=fgetc(fp);
		fscanf(fp,"lf",&data2);
		printf("%lf\n",data2);
	}
	fclose(fp);
	/*FILE *fp;
	fp=fopen("data.txt","wt");
	
	if(fp==NULL)
	{
		//printf("File writing Error!\n");
		puts("File writing Error!\n");
	}
	else
	{
		printf("File writing Success!\n");
	}
	
	fputc('A',fp);
	fputc('B',fp);
	fputc('C',fp);
	fputs("\nDEF\n",fp);
	fclose(fp);
	printf("%d\n",fclose(fp));
	*/
	return 1;
}

void example1()
{
	FILE *fp;
	fp=fopen("data.txt","wt");
	
	if(fp==NULL)
	{
		//printf("File writing Error!\n");
		puts("File writing Error!\n");
		return ;
	}
	else
	{
		printf("File writing Success!\n");
	}
	
	fputc('A',fp);
	fputc('B',fp);
	fputc('C',fp);
	fputs("\nDEF\n",fp);
	//fprintf(fp,'Z'); printf를 더 선호
	fclose(fp);
	printf("%d\n",fclose(fp));	
	return ;
}


void example2 ()
{
	FILE *fp;
	int i;
	char ch;
	fp=fopen("data.txt","rt");
	
	if(fp==NULL)
	{
		//printf("File writing Error!\n");
		puts("File reading Error!\n");
		return ;
	}
	else
	{
		printf("File reading Success!\n");
	}
	
	for(i=0;i<4;i++)
	{
		//ch=fgetc(fp);
		fscanf(fp,"%c",&ch);
		printf("%c\n",ch);
	}
	fclose(fp);
	/*FILE *fp;
	fp=fopen("data.txt","wt");
	
	if(fp==NULL)
	{
		//printf("File writing Error!\n");
		puts("File writing Error!\n");
	}
	else
	{
		printf("File writing Success!\n");
	}
	
	fputc('A',fp);
	fputc('B',fp);
	fputc('C',fp);
	fputs("\nDEF\n",fp);
	fclose(fp);
	printf("%d\n",fclose(fp));
	*/
	return ;
}
