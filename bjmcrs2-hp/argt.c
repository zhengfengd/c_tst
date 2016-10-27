#include <stdio.h>

int main(int argc, char **argv)
{
	//char buf[256]={0};
	char buf[]="you are a hacker with bomb hiden in my code, are you?";
	int i=0;

	printf("test\n");
	(void) argc;
	(void) argv;
	sprintf(buf, "%#f", 136.456);
	printf("%s\r\n", buf);
	sprintf(buf, "who are you ??%#f");
	while (i++ <100){
		printf("step%d: %s\r\n", i, buf);
		sleep(1);
	}
	return 0;
}
