#include <stdlib.h>
#include <string.h>


#define TEST_S "abcdefghijklmnopqrst";

int printb ( int size )
{
	char buf[size];
	strncpy(buf, TEST_S, size -1);
	printf("%s\n", buf);
	return 0;
}

int main(void) 
{
	int size;
	printf("Input an integar:");
	scanf("%d", &size);
	printb(size);
	
	return 0;
}
