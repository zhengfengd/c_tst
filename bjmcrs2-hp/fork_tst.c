#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g = 3;

int main( int argc, char **argv )
{
	int t =1;
	static s= 2;
	char *str;
	str = (char*)malloc(20);
	if(!str){
		printf("Fail to alloc\n");
		return -1;
	}
	strcpy(str,  "HowAreYou?");
	printf("Before fork: t=%d, s=%d, g=%d, str(%p)=%s\n", t, s, g, str, str);
	if(fork()==0){
		t = 11, s=22, g=33;
		sleep(1);
		printf("Child: t=%d, s=%d, g=%d, str(%p)=%s\n", t, s, g, str, str);
	}
	else{
		t = 111, s=222, g=333;
		str[0]='N';
		printf("Parent: t=%d, s=%d, g=%d, str(%p)=%s\n", t, s, g, str, str);
	}
	
	printf("End: t=%d, s=%d, g=%d, str(%p)=%s\n", t, s, g, str, str);
	
	return 0;
	
	
}