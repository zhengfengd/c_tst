#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define BUF_LEN 16

int main (void)
{
	FILE *fp, *fp2;
	char *buf;
	int len, total, i;
	
	len = total = 0;
	
	buf = (char *)malloc(BUF_LEN);
	if(buf<=NULL){
		printf("Failed to malloc!\n");
		return -1;
	}
	memset(buf, 0, BUF_LEN);
	#if 0
	fp = fopen(0, "r");
	if( fp == NULL ){
		printf("Failed to open STDIN\n");
		goto err1;
	}
	#endif
	
	while(1){
		//len = fread(buf, sizeof(char), BUF_LEN, fp);
		fp2 = fgets(buf, BUF_LEN, stdin);
		if(!fp2){
			//printf("Failed to read from *p, len=%d\n", fp, len);
			goto err2;
		}
		if(fp2==EOF){
			printf("Strange, STDIN is EOF\n");
			goto err2;
		}
		//total +=len;
		i=0;
		while(buf[i]!='\0'){
			if( buf[i]>='A'&&buf[i]<='Z' ){
				buf[i] += ('a'-'A')  ;
			}else if(buf[i]>='a'&&buf[i]<='z'){
				buf[i] -= ('a'-'A')  ;
			}
			i++;			
		}
		printf("%s", buf);
	}
	
	return 0;       
	
	err2:
		//fclose(fp);
	err1:
		free (buf);	
		
		return -2;
}
