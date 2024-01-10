#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>



int main()
{
	float L, Z, XC, f, Vmax, Imax, theta, VRmax, VLmax, VCmax = 0.0;
	float R, C, XL;

	L = 0.6;
	Vmax = 150;
	f = 60.0;
	printf("R (Ohm): ");
	scanf_s("%f", &R);
	printf("C (Farad): ");
	scanf_s("%f", &C);

	XL = 2 * M_PI * f * L;
	XC = 1 / (2 * M_PI * f * C);
	Z = sqrt((R * R) + ((XL - XC) * (XL - XC)));
	Imax = Vmax / Z;
	theta = atan((XL - XC) / R) * (180 / M_PI);
	VRmax = Imax * R;
	VLmax = Imax * XL;
	VCmax = Imax * XC;

	printf("유도 리액턴스(XL): %f (Ohm) \n", XL);
	printf("용량 리액턴스 (XC) : %f (Ohm) \n", XC);
	printf("최대전류 (Imax) : %f(A)\n", Imax);
	printf("임피던스 (Z) : %f (Ohm) \n", Z);
	printf("위상각 (theta): %f (degree)\n", theta);
	printf("VRmax : %f (Volt)\n", VRmax);
	printf("VLmax : %f (Volt)\n", VLmax);
	printf("VCmax : %f (Volt)\n", VCmax);
}
