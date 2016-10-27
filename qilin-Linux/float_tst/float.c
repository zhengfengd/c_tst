#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main( int argc, char **argv)
{
	float in;
	int in2, in3;
	while(1){
		printf("Please input a float:");
		scanf("%f", &in);
		printf("You input :%f, int format:%d, round(f)=%f \n", in, in, roundf(in));
		in2 = roundf(in);
		in3 = (int)(in);
		printf("Now round value is %d, force int : %d\n", in2, in3);
	}
	
	return 0;
}
