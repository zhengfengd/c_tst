#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <assert.h>

#define BUF_LEN 16
#define BIT_FLOW
typedef unsigned char UCHAR;

int main (void)
{
	FILE *fp, *fp2;
	char *buf;
	int len, len2, total, i;
	
	len = total = 0;
	
	buf = (char *)malloc(BUF_LEN);
	if(buf<=NULL){
		fprintf(stderr, "Failed to malloc!\n");
		return -1;
	}
	memset(buf, 0, BUF_LEN);
	#if 0
	fp = fopen(0, "r");
	if( fp == NULL ){
		fprintf(stderr, "Failed to open STDIN\n");
		goto err1;
	}
	#endif
	
	while(1){
	#ifdef BIT_FLOW
		len = fread(buf, sizeof(char), BUF_LEN, stdin);
		if(len<=0){
			fprintf(stderr, "Failed to read from %p, len=%d\n", stdin, len);
			goto err2;
		}
		if(fp2==EOF){
			fprintf(stderr, "Strange, STDIN is EOF\n");
			goto err2;
		}
		total +=len;
	#else
		fp2 = fgets(buf, BUF_LEN, stdin);
		if(!fp2){
			//fprintf(stderr, "Failed to read from *p, len=%d\n", fp, len);
			goto err2;
		}
		if(fp2==EOF){
			fprintf(stderr, "Strange, STDIN is EOF\n");
			goto err2;
		}
	#endif
		
		i=0;
		while(buf[i]!='\0'){
			if( buf[i]>='A'&&buf[i]<='Z' ){
				buf[i] += ('a'-'A')  ;
			}else if(buf[i]>='a'&&buf[i]<='z'){
				buf[i] -= ('a'-'A')  ;
			}
			i++;			
		}
	#ifdef BIT_FLOW
		len2 = fwrite(buf, sizeof(UCHAR), len, stdout);
		assert(len == len2);
	#else	
		printf("%s", buf);
	#endif
	}
	
	return 0;       
	
	err2:
		//fclose(fp);
	err1:
		free (buf);	
		
		return -2;
}
