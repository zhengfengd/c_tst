/*
* [usage] ./rdisp line colum line_step colum step freq_ms
  [tips]: use small freq_ms to make the change more motional, use line_step/colum_step to make display rythm versatile 
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#define PROB_NUM 20
#define LINE_NUM 10
#define COLUM_NUM 10
#define BLINK_SPAN 1
#define DIVID_COFF_X_DFT 4
#define DIVID_COFF_Y_DFT 3

#define TEST_LOOP 5000

typedef unsigned char UCHAR;

unsigned char g_pic[LINE_NUM][COLUM_NUM]={0};

int generate_random( UCHAR *buf, int line, int colum)
{
	int i, j;
	
	assert( line!=0 && colum!=0 );
	for( i=0; i<line; i++)
	  for( j=0; j<colum; j++)
	  {
	  	buf[ i*line + j ] = random()%PROB_NUM;
	  }
	
	return 0;
}

typedef enum{
	NO_OVERFLOW,
	ONLY_X_OVERFLOW,
	ONLY_Y_OVERFLOW,
	X_Y_OVERFLOW
}EFLAG;

typedef enum{
	PRINTF,
	PUTCHAR
}EMODE;

int setRandomValue(UCHAR *ch )
{
	if(*ch == 1|| *ch == 2 || *ch == 3)
		*ch += 1;
	else{
		*ch = random()%PROB_NUM;
		if( *ch == 2 || *ch == 3)
		    *ch = 0;
	}
	return 0;
}

/*
   Decrease the flashing point
*/
UCHAR getDispChar( UCHAR c)
{
	switch(c){
	case 1:
		return 'X';
		break;
	case 2:
		return 'x';
		break;
	case 3:
		return '.';
		break;
	case 0:
	default:
		return ' ';
		break;
	}
}


int generate_random_p( UCHAR *buf, int line_b, int line_e, int line, int colum_b, int colum_e, int colum)
{
	int i, j;
	EFLAG flag;
	
	if( line_b<line_e && colum_b<colum_e )
		flag = NO_OVERFLOW;
	else if(line_b>=line_e  && colum_b<colum_e )
		flag = ONLY_X_OVERFLOW;
	else if( line_b<line_e && colum_b>=colum_e )
		flag = ONLY_Y_OVERFLOW;
	else if(line_b>=line_e  && colum_b>=colum_e )
		flag = X_Y_OVERFLOW;
	else{
		printf("line%d:wrong branch\n", __LINE__);
		assert(0);		
	}
	
	switch(flag){
	case 	NO_OVERFLOW:
		for( i=line_b; i<line_e; i++)
		  for( j=colum_b; j<colum_e; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		break;
	case 	ONLY_X_OVERFLOW:
		{
		for( i=line_b; i<line; i++)
		  for( j=colum_b; j<colum_e; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		  
		for( i=0; i<line_e; i++)
		  for( j=colum_b; j<colum_e; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		}
		break;
	case 	ONLY_Y_OVERFLOW:
		{
		for( i=line_b; i<line_e; i++)
		  for( j=colum_b; j<colum; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		  
		for( i=line_b; i<line_e; i++)
		  for( j=0; j<colum_e; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		}
		break;
	case 	X_Y_OVERFLOW:
		for( i=line_b; i<line; i++)
		  for( j=colum_b; j<colum; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		for( i=0; i<line_e; i++)
		  for( j=0; j<colum_e; j++)
		  {
		  	//buf[ i*line + j ] = random()%PROB_NUM;
		  	setRandomValue( &buf[ i*line + j ] );
		  }
		break;
	default:
		printf("line%d:wrong branch\n", __LINE__);
		assert(0);
		
	}
		
	return 0;
}

int print_buf( UCHAR *buf, int line, int colum)
{
	int i, j;
	
	assert( line!=0 && colum!=0 );
	for( i=0; i<line; i++)
	{
	  if( i==0 ){
	  	for( j=0; j<colum*2; j++) printf("%c", '=');
	  	printf("\n");
	  }
	  for( j=0; j<colum; j++)
	  {
	  	printf("%c ", getDispChar( buf[ i*line + j ]) );
	  }
	  printf("\n");
	  if( i == line-1 ){
	  	for( j=0; j<colum*2; j++) printf("%c", '=');
	  	printf("\n");
	  }
	}
	return 0;
}

int put_buf( UCHAR *buf, int line, int colum)
{
	int i, j;
	
	assert( line!=0 && colum!=0 );
	for( i=0; i<line; i++)
	{
	  if( i==0 ){
	  	for( j=0; j<colum*2; j++) putchar('=');
	  	putchar('\n');
	  }
	  for( j=0; j<colum; j++)
	  {
	  	putchar(getDispChar( buf[ i*line + j ]) );
	  	putchar(' ');
	  }
	  putchar('\n');
	  if( i == line-1 ){
	  	for( j=0; j<colum*2; j++) putchar('=');
	  	putchar('\n');
	  }
	}
	return 0;
}

int display_buf( UCHAR *buf, int line, int colum)
{
	return print_buf( buf, line, colum);
}

#ifdef EFFIC_TEST
int calTime(UCHAR *pic, int line, int colum, EMODE mode)
{
	struct timeval begin, end, ret;
	int loop=0, countFlag=1, r;
	FILE *fp;
	
	fp = fopen("result.log", "awr");
	if(!fp){
		printf("Fail to open\n");
		exit (-1);
	}

	if(gettimeofday(&begin, NULL)){
		printf("cannot calculate timer\r\n");	
		countFlag=0;		
	}
	loop = 0;
	if(mode == PRINTF){
		while(loop++<TEST_LOOP)
			print_buf(pic, line, colum);
	}
	else if(mode == PUTCHAR){
		while(loop++<TEST_LOOP)
			put_buf(pic, line, colum);
	}else{
		printf("Fail!! wrong param\n");
		r = -2;
		goto error2;
	}
	if(gettimeofday(&end, NULL)){
		printf("line %d:cannot calculate timer\r\n", __LINE__);	
		countFlag=0;	
	}		
	if( countFlag!=0 )
	{
		if(end.tv_sec - begin.tv_sec < 0)
			printf("Overflow, please retry\r\n");
		ret.tv_sec = end.tv_sec - begin.tv_sec;
			
		if(end.tv_usec - begin.tv_usec < 0 ){
			ret.tv_sec --;
			ret.tv_usec = 1000000 + end.tv_usec - begin.tv_usec;
		}else{
			ret.tv_usec = end.tv_usec - begin.tv_usec;
		}
		fprintf(fp, "%s! Time difference: %d sec, %d usec\n", (mode==PRINTF)?"printf":"putchar", ret.tv_sec, ret.tv_usec );
		fflush(fp);
		printf("%s! Time difference: %d sec, %d usec\n", (mode==PRINTF)?"printf":"putchar", ret.tv_sec, ret.tv_usec );
	}
error2:
	close (fp);
	return r;	
}

int testPrintEff(UCHAR *pic, int line, int colum)
{//diff putchar printf efficiency
	struct timeval begin, end, ret;
	int loop=0, countFlag=1;
	
	generate_random_p(pic, 0, line, line, 0, colum , colum);
	
	calTime( pic,  line, colum, PRINTF);
	sleep(3);
	calTime( pic,  line, colum, PUTCHAR);
	return 0;
}
#endif

int main(int argc, char **argv)
{
	int line, colum, i, j, count, step_x, step_y, ms=0;
	int DIVID_COFF_X, DIVID_COFF_Y;
	UCHAR *pic;
	int loop=0;
	
	if(argc >= 3 ){
		line = atoi( argv[1] );
		colum = atoi( argv[2] );
		assert( line!=0 && colum!=0 );
	}else{
		line = LINE_NUM;
		colum = COLUM_NUM;
	}
	if(argc < 5){
		DIVID_COFF_X = DIVID_COFF_X_DFT;
		DIVID_COFF_Y = DIVID_COFF_Y_DFT;
	}else{
		DIVID_COFF_X = atoi(argv[3]);
		DIVID_COFF_Y = atoi(argv[4]);
	}
	if(argc > 5){
		ms = atoi(argv[5]);
	}
	pic = (UCHAR *)malloc(line*colum);
	if(!pic){
		printf("line%d: fail to malloc\n", __LINE__);
		goto error1 ;
	}
	
	i=0;
	j=0;
	count = 0;
	step_x = 0;
	step_y = 0;
	step_x = line / DIVID_COFF_X;
	step_y = colum / DIVID_COFF_Y;
#ifdef EFFIC_TEST
	{	
		testPrintEff( pic, line, colum);
		return 0;
	}
#endif
	while( 1 ){
		//generate_random(pic, line, colum);
		generate_random_p(pic, i, (i+step_x)%line, line, j, (j+step_y)%colum , colum);
		i = (i+step_x)%line;
		j = (j+step_y)%colum;
		display_buf(pic, line, colum);
		if( ms!=0 )
		{
			 struct timespec interval, remainder;		
		           interval.tv_sec = 0;
		           interval.tv_nsec = ms*1000000;
		           if (nanosleep(&interval, &remainder) == -1) {
		               if (errno == EINTR) {
		                   (void)printf("nanosleep interrupted\n");
		                   (void)printf("Remaining secs: %d\n", remainder.tv_sec);
		                   (void)printf("Remaining nsecs: %d\n", remainder.tv_nsec);
		               }
		               else perror("nanosleep");
		           }
		}else{
			sleep(BLINK_SPAN);
		}
		/*if(count++ %9 == 8){
			generate_random(pic, line, colum);
			display_buf(pic, line, colum);
		}*/
	}

	
error1:
	free(pic);
	
	return 0;
	
}
 