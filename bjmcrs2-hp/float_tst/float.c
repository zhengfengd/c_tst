#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main( int argc, char **argv)
{
	//double in;
	float in;
	int in2, in3;
	while(1){
		printf("sizeof(float)=%d, sizeof(double)=%d\n", sizeof(float), sizeof(double));
		printf("Please input a float:");
		//scanf("%lf", &in);
		scanf("%f", &in);
		printf("You input :%lf, int format:%d, round(f)=%f \n", in, in, round(in));
		in2 = round(in);
		in3 = (int)(in);
		printf("Now round value is %d, force int : %d\n", in2, in3);
	}
	
	return 0;
}
