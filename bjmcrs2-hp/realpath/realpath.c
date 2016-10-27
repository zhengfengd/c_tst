#include <stdlib.h>
#include <errno.h>

#define PATH_MAX 1024

int main(int argc, char **argv)
{
	char path[PATH_MAX+1], result[PATH_MAX+1];
	char *ret;
	int len;
	
	if(argc!=2){
		printf("Wrong param: realpath <path_name>\n");
		exit -1;
	}
	if( (len= strlen(argv[1])) > PATH_MAX ){
		printf("Too long path, you should know what you did \n");	
		return -2;
	}
	strcpy(path, argv[1]);
	path[len] = '\0';
	ret = realpath(path, result);
	if(!ret){
		printf("Fail: errno = %d\n", errno);	
	}
	printf("Input is %s\n", path);
	printf("result is %s\n", result);
	printf("ret is %s\n", ret);
	
	return 0;
}
