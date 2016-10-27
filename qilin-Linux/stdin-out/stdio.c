#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <assert.h>

#define BUF_LEN 16
typedef unsigned char UCHAR;

int main ( int argc, char **argv )
{
	//FILE *fp, *fp2;
	char *buf;
	int len, len2, total, i;
	
	len = total = 0;
	
	buf = (char *)malloc(BUF_LEN);
	if(buf<=NULL){
		fprintf(stderr, "Failed to malloc!\n");
		return -1;
	}
	memset(buf, 0, BUF_LEN);
	
	while(1){
		len = fread(buf, sizeof(char), BUF_LEN, stdin);
		if(len<=0){
			fprintf(stderr, "Failed to read from %p, len=%d\n", stdin, len);
			goto err2;
		}
		total +=len;
		
		i=0;
		while(buf[i]!='\0'){
			if( buf[i]>='A'&&buf[i]<='Z' ){
				buf[i] += ('a'-'A')  ;
			}else if(buf[i]>='a'&&buf[i]<='z'){
				buf[i] -= ('a'-'A')  ;
			}
			i++;			
		}
		len2 = fwrite(buf, sizeof(UCHAR), len, stdout);
		assert(len == len2);
	}
	
	return 0;       
	
	err2:
		//fclose(fp);
	err1:
		free (buf);			
		return -2;
}
