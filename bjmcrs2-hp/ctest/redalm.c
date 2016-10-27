#include <stdio.h>

int print(char*fmt, ...)
{
	int a;
	a++;
	printf("%d, %s\n", a, fmt);
	return 0;
}

int main(int argc, char **argv)
{
	char a[]="1234";
	char b[]="5678";
	char c[]="90ab";
	char str[]="a=%x,b=%x\n";
	printf("I am back, %s\n","tudou");
	printf(str);
	print(str);
		
	return 0;
}
