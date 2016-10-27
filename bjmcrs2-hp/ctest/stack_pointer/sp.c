#include <stdio.h>
int fun1(char str[1024])
{
	printf("sizeof(str) = %d\n", sizeof(str));
	return 0;
}

int main()
{
	char a[1024];
	fun1(a);
	return 0;
	
	}
