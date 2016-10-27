#include <stdio.h>
#define LEN 16
#define TLS1_HB_RESPONSE 2

int main( int argc, char **argv )
{

	unsigned char *buffer, *bp;  
    int r, i;  
	
    buffer = malloc(LEN); 
    memset(buffer, 0xee, LEN);
     
    bp = buffer; 

    /* Enter response type, length and copy payload */ 
    *bp++ = TLS1_HB_RESPONSE;  
    printf("buffer (%p), bp (%p): \n", buffer, bp);
    for (i = 0; i<LEN; i++){
    	printf("%02x ", *(buffer+i));
    }
    
    return 0;

}