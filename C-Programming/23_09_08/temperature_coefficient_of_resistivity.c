#include<stdio.h>
#include<math.h>
#define Temperature_coefficient_of_resistivity_Point 7

struct 
{
	double X[Temperature_coefficient_of_resistivity_Point];
	double Y[Temperature_coefficient_of_resistivity_Point];
}cal_data = 
	{
		{106.0,315.0,481.0,736.0,936.0,1083.0,1232.0},
		{1.56,2.67,3.55,4.90,5.95,6.74,7.53} 
	};

double linear_mapping(double R) 
{
	int i, j;
	double x1 = cal_data.X[i];
	double x2 = cal_data.X[i + 1];
	double y1 = cal_data.Y[i];
	double y2 = cal_data.Y[i + 1];
	double t;
	double R0 = 1.0;

	for (j = 0; j < Temperature_coefficient_of_resistivity_Point-1; j++) 
	{
		if (R < cal_data.X[0])
		{
			i = 0;
		}
		else if( (R >= cal_data.X[j]) && (R < cal_data.X[j + 1]))
		{
			i = j;
		}
		else 
		{
			i = Temperature_coefficient_of_resistivity_Point-2;
		}
	}
	double m = ((y2 - y1) / (x2 - x1));
	t = (R - R0)/m;
	return t;
}

int main()
{
	double R = 3.0/(340*pow(10.0,-3.0));
	printf("R = %lf (ohm)\nt = %lf(Celsius)\n",R,linear_mapping(R));
	return 0;
}
