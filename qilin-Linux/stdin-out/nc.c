#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <libgen.h>


#define BUF_LEN 1024
#define SERVER_ACCEPT_BACKLOG 5
//#define DBG
#define SELECT_TIME_OUT 15

typedef unsigned char UCHAR;

typedef struct sockaddr_in SOCKADDR;

typedef struct global_param{
	int server_mode;
	int verbose_mode;
	SOCKADDR sock_addr;
	int cmdMode;
	char *cmd;
} G_PARAM;


G_PARAM globalArgs={0};

#ifdef DBG
#define dbg(format, arg...) \
do { \
  fprintf(stdout, format, ## arg); \
} while (0) \

#else
#define dbg(format, arg...) do {} while (0)

#endif


void display_usage( FILE *stream, int errCode )
{
	fprintf( stream, "nc [address] [-l] [-p] [port_num] [-e] [cmd]\n");	
	exit(errCode);
}

int displayGlobalParam( FILE *stream, int errCode )
{
	fprintf(stream, "server_mode:%s, verbose_mode:%s, addr:%s, port:%d, cmd_mode:%s, cmd:%s\n", 
		globalArgs.server_mode==1?"TRUE":"FALSE", 
		globalArgs.verbose_mode==1?"TRUE":"FALSE", 
		inet_ntoa( globalArgs.sock_addr.sin_addr ),
		ntohs(globalArgs.sock_addr.sin_port),
		globalArgs.cmdMode==1?"TRUE":"FALSE",
		globalArgs.cmd=NULL?"": globalArgs.cmd);	
	return errCode;
}

	//1. arguments option parsing
int handleArgumentOptions( int argc, const char * argv[] )
{
	int opt =0, i;
	const char* const short_options = "lp:e:hv"; 
	const struct option long_options[] = {
		{ "listen",    0, NULL, 'l' }, 
		{ "port",    1, NULL, 'p' }, 
		{ "exec",    1, NULL, 'e' }, 
		{ "help",    0, NULL, 'h' }, 
		{ "verbose", 0, NULL, 'v' }, 
		{ NULL,       0, NULL, 0   }   /* Required at end of array.  */ 
	}; 
	struct hostent *hostinfo;
	
	do{
		opt = getopt_long( argc, argv, short_options, long_options, NULL );
		switch( opt ) {
		        
		    case 'l':
		        globalArgs.server_mode = 1;		        
		        break;
		        
		    case 'p':
		        //globalArgs.port = atoi(optarg);
		        globalArgs.sock_addr.sin_port = htons(atoi(optarg));
		        assert( globalArgs.sock_addr.sin_port > 0 );
		        break;
		        
		    case 'h':   /* fall-through is intentional */
		        display_usage(stdout, 0);
		        break;
		    case '?':
		        display_usage(stderr, -1);
		        break;
		    
		    case 'v':
		    	globalArgs.verbose_mode = 1;
		        break;
		    
		    case 'e':
		    	globalArgs.cmdMode = 1;
		    	globalArgs.cmd = optarg;
		        break;
		    
		    case -1:
		    	break;
		        
		    default:
		        display_usage(stderr, -2);
		        break;
		}
	}while( opt != -1 );
	
	//globalArgs.sock_addr.sin_addr.s_addr = inet_addr(argv[optind]); //address
	if(globalArgs.server_mode == 1){
		globalArgs.sock_addr.sin_addr.s_addr = htons(INADDR_ANY);
	}else{
		hostinfo = gethostbyname (argv[optind]); 
		if (hostinfo == NULL) {
			fprintf(stderr, "Wrong hostname address: %s\n", argv[optind]);
	     		return -1; 
	     	}else{
	     		dbg("IP Address : %s\n",inet_ntoa(*((struct in_addr *)hostinfo->h_addr)));
	    		globalArgs.sock_addr.sin_addr = *((struct in_addr *) (hostinfo->h_addr));  //address
	    	}
	}
	for (i = optind; i < argc; ++i) {
		dbg ("%s: Argument: %s\n", __FUNCTION__, argv[i]); 
	}
	if(globalArgs.verbose_mode == 1){
		displayGlobalParam( stdout, 0 );	
	}
	
	return 0;
}	
	
	//2. net link setup
int netLinkSetup()
{
	static int local_sock_fd, peer_sock_fd;
	int sin_size, ret;
	static SOCKADDR peerSock;
	static fd_set ins, outs;
	
	FD_ZERO(&ins);
	FD_ZERO(&outs);
	
	local_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if( local_sock_fd < 0 ){
		fprintf(stderr, "Failed to create socket\r\n");	
		return -1;
	}
	globalArgs.sock_addr.sin_family = AF_INET;
	bzero( &(globalArgs.sock_addr.sin_zero), 8 );
	if(globalArgs.server_mode == 1 ){
		//bind
		
	        ret = bind(local_sock_fd, (struct sockaddr *)&globalArgs.sock_addr, sizeof(struct sockaddr));
	        if(ret <0){
	        	fprintf(stderr, "Failed to bind socket\r\n");	
			return -2;
	        }
	        //listen
	        ret = listen( local_sock_fd, SERVER_ACCEPT_BACKLOG );
	        if( ret < 0 )
	        {
	        	fprintf(stderr, "Failed to listen socket, errno=%d\r\n", errno);	
			return -3;
	        }
	
	        sin_size = sizeof( struct sockaddr_in );
	        peer_sock_fd = accept(local_sock_fd, &peerSock, &sin_size);
	        if( peer_sock_fd < 0 )
	        {
	        	fprintf(stderr, "Failed to accept socket, errno=%d\r\n", errno);	
			return -4;
	        }
	        dbg("Succeed to create server communication socket\r\n");
	        return peer_sock_fd;//return communication socketfd;
			
	}else{
		//sock
		ret = connect(local_sock_fd, (struct sockaddr *)&globalArgs.sock_addr, sizeof(struct sockaddr) );
		if( ret < 0 ){
			fprintf(stderr, "%s $s:fail to connect remote peer, errno=%d\r\n", __FUNCTION__, __LINE__, errno);	
			return -5;
		}
		dbg("Succeed to create client communication socket\r\n");
		return local_sock_fd;		
	}
}
	
	//3. stdin receive and sent to netlink
int data_hub( int comm_sock_id  )
{
	static fd_set ins, outs;
	struct timeval delayer;
	int fd_stdin, fd_stdout;
	int ret, ret2;
	UCHAR buf[BUF_LEN];
	
	fd_stdin = 0;//STDIN_FILENO
	fd_stdout = 1;//STDOUT_FILENO
	
	FD_ZERO(&ins);
	FD_ZERO(&outs);
	
	delayer.tv_sec = SELECT_TIME_OUT;
	delayer.tv_usec = 0;
	FD_SET(fd_stdin, &ins);
	FD_SET(comm_sock_id, &ins);
	
	do{
		FD_ZERO(&ins);
		FD_ZERO(&outs);
		
		delayer.tv_sec = SELECT_TIME_OUT;
		delayer.tv_usec = 0;
		FD_SET(fd_stdin, &ins);
		FD_SET(comm_sock_id, &ins);
		ret = select( comm_sock_id+1, &ins, NULL, NULL, &delayer );
		if(ret<0){
			fprintf(stderr, "%s %d:fail to select, errno=%d\r\n", __FUNCTION__, __LINE__, errno);	
			return -5;
		}else{
			dbg("return of select(): %d\n", ret);
		}
		
		if(FD_ISSET(fd_stdin, &ins)){// receive some input from fd_stdin
			ret = read(fd_stdin, buf, sizeof(buf));
			if(ret<=0){
				fprintf(stderr, "%s %d:Failed to read from %d, len=%d\n", __FUNCTION__, __LINE__, fd_stdin, ret);
				return -1;
			}
			
			//ret2 = send(comm_sock_id, buf, ret, 0);
			ret2 = write(comm_sock_id, buf, ret);
			if(ret2<=0){
				fprintf(stderr, "%s %d:Failed to write to %d, len=%d\n", __FUNCTION__, __LINE__, comm_sock_id, ret2);
				return -2;
			}
			
		}
		if(FD_ISSET(comm_sock_id, &ins)){// receive sth from remote peer via socket
			//ret = recv(comm_sock_id, buf, sizeof(buf), 0);
			ret = read(comm_sock_id, buf, sizeof(buf));
			if(ret<=0){
				fprintf(stderr, "%s %d:Failed to read from socket fd %d, len=%d\n", __FUNCTION__, __LINE__, comm_sock_id, ret);
				return -3;
			}
			ret2 = write(fd_stdout, buf, ret);
			if(ret2<=0){
				fprintf(stderr, "%s %d:Failed to write to %d, len=%d\n", __FUNCTION__, __LINE__, fd_stdout, ret2);
				return -4;
			}
		}
	}while(1);
	
	return 0;
}
	//4. netlink recv and sent to stdout
	
int handle_nc_exec( int sock_fd )
{
	int fd;
	dup2(2, fd); //backup stderr
	dup2(sock_fd, 0); //redirect STDIN to sock fd;
	dup2(sock_fd, 1); //redirect STDOUT to sock fd;
	fprintf(stdout, "Welcome to the new world from exec() peer\n");
	execl( globalArgs.cmd, basename(globalArgs.cmd), NULL );
	dup2(fd, 2);
	fprintf( stderr, "fatal error to run execl(), errno=%d\n", errno);
	return -1;	
}

int main ( int argc, char *argv[] )
{
	//FILE *fp, *fp2;
	char *buf;
	int len, len2, total, i, ret;
	int comm_sock_id;
	//1. arguments option parsing
	ret = handleArgumentOptions(argc, argv);
	if (ret<0)
		goto err2;
		
	//2. net link setup
	comm_sock_id = netLinkSetup();
	if ( comm_sock_id<0 )
		goto err2;
	
	if(globalArgs.cmdMode == 1){
		handle_nc_exec(comm_sock_id);		
	}	
	//3. stdin receive and sent to netlink
	
	//4. netlink recv and sent to stdout
	ret = data_hub( comm_sock_id );
	dbg("End of data hub, what's matter? ret:%d\n", ret);
	
	return 0;       
	
	err2:
		//fclose(fp);
	err1:
		free (buf);			
		return -2;
}

