#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main( int argc, char **argv )
{
	char *ret;
	if(setlocale(LC_CTYPE, "") == NULL)  
	//if((ret = setlocale(LC_ALL, "zh_CN.UTF-8")) == NULL)  
	{ /*设置为本地环境变量定义的locale*/  
		fprintf(stderr, "can't set the locale\n");  
	} else
	{
		fprintf(stdout, "ret is %s\n", ret);  
	}
	wchar_t hi[]=L"你是谁？";	
	wprintf(L"%ls\n", hi);
	wprintf("%wc\n", 0x7ed3);
	
	return 0;
	
}
                                                                                                             