#include <stdio.h>
#include <assert.h>
#include <string.h>

#define TEST_LOOP

#define CHECK_CHAR(a) ('a'<(a)<'z' || 'A'<(a)<'Z' || '0'<(a)<'9')
	#define MAX_LEN 64

/*
2. compare the whole string
	get the pin from the passwd file, and fetch the length;
	construct a whole string of that length;
	compare with the string.


	
	num = fread(pdbuf, 1, 64, fp);
	if(num <=0){
		printf("error to read passwd\n");
		return -1;
	}
	size = strlen(pdbuf);
	assert (num == size);
	
	1. loop to construct a string with that size;
	2. loop to compare with the buff;
*/	


int rcr_cstr(char *str, int loop, int size, char *psw, int *result) 
{
	static int count = 0;
	assert(psw!=NULL);
	for(str[size-loop]='?'; str[size-loop]<='z';str[size-loop]++)
	{
		if(loop<=1 ){
		#if 0
			printf("%s\t", str);
			if(count++%8 == 0) printf("\n");
		#else			
			if(!strncmp(str, psw, size)){
				printf("Yes, found: str=%s, psw=%s, loop=%d, size=%d, total:%d times\n", str, psw, loop, size, count);
				*result = 1;
				break;
			}
			else{
				count++;
				if(count%10000 == 0)printf("*");
			}
		#endif
		}
		else{
			rcr_cstr(str, loop-1, size, psw, result);
			if(*result == 1)
				break;
		}
	}
	return 0;
}

int main(int argc, char **argv )
{
 FILE *fp;
 char pd, c, *o;
 
 if(argc >1)
  fp = fopen(argv[1], "r+");
 else
  fp = fopen("pd.txt", "r+");
 if(fp <= NULL)
 {
  printf("Error to read ps file\n"); 
  return -1;
 }
#ifdef TEST1
 while(fscanf(fp, "%c", &pd)!=EOF){
  for (c='0';c<='z'; c++)
  {
   /*if( !CHECK_CHAR(c) )
    continue;*/
   if(c == pd){
    printf("%c", c); 
    break;
	}
  }
  if((c>'z') &&(pd!= 0x0a)){
   printf("\nUn-char(%02x), not understand\n", pd);
   return -2;
  }
 } 
#else
{
 	char str[MAX_LEN] = {0}, pwd[MAX_LEN]={0};	
 	int loop, size, ret = 0;
 	fscanf(fp, "%s", pwd);
 	printf("Answer should be \"%s\"\n", pwd);
 	if(argc >2)
 		size = loop = atoi(argv[2])	;
 	else 
 		size = loop = strlen(pwd);
 	rcr_cstr(str, loop, size, pwd, &ret);
 	if(ret == 1)
 		printf("\nYes, found: str=\"%s\", psw=\"%s\", loop=%d, size=%d\n", str, pwd, loop, size);
 	else
 		printf("\nFail to get the passwd: str=\"%s\", psw=\"%s\", loop=%d, size=%d\n", str, pwd, loop, size);
 }		
#endif
	 printf("\n");
 fclose(fp);
 #if 0
 {
 	char str[MAX_LEN] = {0};	
 	int loop, size;
 	if(argc >1)
 		size = loop = atoi(argv[1])	;
 	else 
 		size = loop = 3;
 	rcr_cstr(str, loop, size, "pass");
 	printf("\n");
 }
 #endif
 return 0;
}                                
