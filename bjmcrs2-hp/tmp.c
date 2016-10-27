/*
sizeof(float):4, sizeof(double):8, sizeof(long double):16, 
#define PI, is double by default
scanf("%")
*/

#include <stdio.h>

#define DOUBLE_PRECISION

#ifndef DOUBLE_PRECISION
	#define PI 3.14159265359
#else
	#define PI 3.14159265359L
#endif

int main(int argc, char ** argv)
{
#ifndef DOUBLE_PRECISION
	float degree = 30;
	float radian = 0, tmp2;
#else
	long double degree = 30;
	long double radian = 0, tmp2;
#endif
	long double rad2 = 0, temp;
	
	printf("Input degree:");
#ifndef DOUBLE_PRECISION
	scanf("%f", &degree);  // !! big different for "%f" and "%lf"
	//scanf("%lf", &degree);
#else
	scanf("%llf", &degree);
#endif	
	printf("sizeof(float)=%d,sizeof(long double)=%d,sizeof(double)=%d\n", sizeof(float),sizeof(long double),sizeof(double));
	printf("Line %d: sizeof(PI)=%d,sizeof(degree)=%d, sizeof(radian)=%d \n", __LINE__, sizeof(PI), sizeof(degree), sizeof(radian));
	radian = degree * PI /180;
	printf("sizeof(degree * PI /180)=%d, degree=%f\n", sizeof(degree * PI /180), degree);
	printf("Line%d:radian = %f, %lf\n", __LINE__, radian, degree * PI /180.0);
	
	radian = degree * (float)PI /180;
	rad2  = degree * (long double)PI /180;
	printf("Line%d:PI=%lf, radian = %f, rad2 = %lf, %lf\n", __LINE__, PI, radian, rad2, degree * PI /180.0);
	temp = (long double) PI;
	tmp2 = (float)PI;
	printf("Line%d: temp=%lf, tmp2=%f, radian = %f, rad2 = %lf, %lf\n", __LINE__, temp, tmp2, radian, rad2, degree * PI /180.0);
	
	printf("Line %d: sizeof(PI)=%d,sizeof(degree)=%d, sizeof(radian)=%d \n", __LINE__, sizeof(PI), sizeof(degree), sizeof(radian));
	return 0;
}