#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main ( int argc , char **argv )
{
	int ret,fp,fd;
	char cs[2];
	char *filebuff;
	struct stat buf;
	size_t filelen;
	
	if ( stat(argv[1],&buf) == 0 )
		filelen = buf.st_size ;
	else
		filelen = 0 ;
		
	if ( (filebuff = malloc(sizeof(char) * filelen )) == NULL )
	{
		perror("Malloc memory error !\n");
		exit( -1 );
	}
	bzero(filebuff, filelen );
	if ( ( fp = open ( argv[1] ,O_RDONLY ) ) == NULL )
	{
		free(filebuff);
		perror("This argv[1] error !\n");
		exit(-1);
	}
	
	ret = 0;
	while( read( fp , cs , 1) != 0 )
	{
		if ( cs[0] != '\015' )
		{
			*(filebuff+ret) = cs[0];
			ret++;
		}
	}
	close(fp);
	
	if ( (fd = open ( argv[1] ,O_WRONLY | O_CREAT | O_TRUNC )) == NULL )
	{
		free(filebuff);
		perror("Open File argv[1] error !\n");
		exit(-1);
	}
	
//	fprintf(stderr,"BUFF =[ %s ] \nFILELEN = [ %d ]\n",filebuff,ret);
	write( fd , filebuff , ret );
	close(fd);
	free(filebuff);
	exit(0);
} 
