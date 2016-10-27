#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <unistd.h>

#define LINUX 1

#if defined(LINUX)
#include <getopt.h>
#include <sys/select.h>
#elif defined(UNIX)
#include <unistd.h>
//#include <getopt.h>
#include <sys/time.h>
#endif

#define BUF_LEN 1024
#define SERVER_ACCEPT_BACKLOG 5
//#define DBG
#define SELECT_TIME_OUT 15

typedef unsigned char UCHAR;

typedef struct sockaddr_in SOCKADDR;

typedef struct global_param{
	int server_mode;
	int client_mode;
	//int client_port;
	int verbose_mode;
	SOCKADDR sock_addr;
	SOCKADDR client_addr;
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
	fprintf( stream, "udptst [address] [-l] [-p] [server_port_num] [-s] [src_port_num] [-v]\n");	
	exit(errCode);
}

int displayGlobalParam( FILE *stream, int errCode )
{
	fprintf(stream, "server_mode:%s, verbose_mode:%s, addr:%s, port:%d, client_mode:%s [addr:%s, port:%d]\n", 
		globalArgs.server_mode==1?"TRUE":"FALSE", 
		globalArgs.verbose_mode==1?"TRUE":"FALSE", 
		inet_ntoa( globalArgs.sock_addr.sin_addr ),
		ntohs(globalArgs.sock_addr.sin_port),
		globalArgs.client_mode==1?"TRUE":"FALSE",
		globalArgs.client_mode==1?inet_ntoa( globalArgs.client_addr.sin_addr ): "NULL",
		globalArgs.client_mode==1?ntohs(globalArgs.client_addr.sin_port): 0 );	
	return errCode;
}

	//1. arguments option parsing
int handleArgumentOptions( int argc, const char * argv[] )
{
	int opt =0, i;
	const char* const short_options = "lp:e:hs:v"; 
#if defined (LINUX)
	const struct option long_options[] = {
		{ "listen",    0, NULL, 'l' }, 
		{ "port",    1, NULL, 'p' }, 
		{ "src_port_num",    1, NULL, 's' }, 
		{ "help",    0, NULL, 'h' }, 
		{ "verbose", 0, NULL, 'v' }, 
		{ NULL,       0, NULL, 0   }   /* Required at end of array.  */ 
	}; 
#endif
	struct hostent *hostinfo;
	
	do{
#if defined(LINUX)	
		opt = getopt_long( argc, argv, short_options, long_options, NULL );
#elif defined(UNIX)
		opt = getopt(argc, argv, short_options);
#endif
		switch( opt ) {
		        
		    case 'l':
		        globalArgs.server_mode = 1;		        
		        break;
		        
		    case 'p':
		        //globalArgs.port = atoi(optarg);
		        globalArgs.sock_addr.sin_port = htons(atoi(optarg));
		        assert( globalArgs.sock_addr.sin_port > 100 && globalArgs.sock_addr.sin_port < 65555 );
		        break;
		    
		    case 's':
		        //globalArgs.port = atoi(optarg);
		        globalArgs.client_mode = 1;
		        //globalArgs.client_port = atoi(optarg);
		        globalArgs.client_addr.sin_port = htons(atoi(optarg));
		        assert( globalArgs.client_addr.sin_port > 100 && globalArgs.client_addr.sin_port < 65555 );
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
		    		    
		    case -1:
		    	break;
		        
		    default:
		        display_usage(stderr, -2);
		        break;
		}
	}while( opt != -1 );
	
	//globalArgs.sock_addr.sin_addr.s_addr = inet_addr(argv[optind]); //address
	assert((globalArgs.client_mode==0 && globalArgs.server_mode==1)
		||(globalArgs.client_mode==1 && globalArgs.server_mode==0));
	dbg("optind %d, argc=%d\n", optind, argc);
	if( optind < argc ){	//should be only for client	
		hostinfo = gethostbyname (argv[optind]); 
		if (hostinfo == NULL) {
			fprintf(stderr, "Wrong hostname address: %s\n", argv[optind]);
	     		return -1; 
	     	}else{
	     		dbg("IP Address : %s\n",inet_ntoa(*((struct in_addr *)hostinfo->h_addr)));
	    		globalArgs.sock_addr.sin_addr = *((struct in_addr *) (hostinfo->h_addr));  //address
	    	}
	    	globalArgs.client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	}else{
		if(globalArgs.server_mode == 1){
			globalArgs.sock_addr.sin_addr.s_addr = htons(INADDR_ANY);
		}
		else{// for client, address should be provided
			display_usage(stderr, -1);
			return -2;
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
static int local_sock_fd, peer_sock_fd;	
int netLinkSetup()
{
	int sin_size, ret;
	static SOCKADDR peerSock;
	
	
	local_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if( local_sock_fd < 0 ){
		fprintf(stderr, "Failed to create socket\r\n");	
		return -1;
	}
	if( globalArgs.client_mode == 1 ){
		globalArgs.client_addr.sin_family = AF_INET;
		bzero( &(globalArgs.client_addr.sin_zero), 8 );
				
	        ret = bind(local_sock_fd, (struct sockaddr *)&globalArgs.client_addr, sizeof(struct sockaddr));
	        if(ret <0){
	        	fprintf(stderr, "client: Failed to bind socket\r\n");	
			return -2;
	        }
	}else{
		globalArgs.sock_addr.sin_family = AF_INET;
		bzero( &(globalArgs.sock_addr.sin_zero), 8 );
				
	        ret = bind(local_sock_fd, (struct sockaddr *)&globalArgs.sock_addr, sizeof(struct sockaddr));
	        if(ret <0){
	        	fprintf(stderr, "Server: Failed to bind socket\r\n");	
			return -3;
	        }
	}
        return local_sock_fd;//return communication socketfd;
			
}
int handle_data( UCHAR *buf)
{
	while(*buf){
		if(*buf>='a'&& *buf<='z'){
			*buf -= ('a'-'A');
		}else if(*buf>='A'&& *buf<='Z'){
			*buf += ('a'-'A');
		}
		buf++;
	}
	return 0;	
}
int data_hub( int comm_sock_id  )
{
	struct timeval delayer;
	int ret, ret2, addr_len;
	UCHAR sendmsg[BUF_LEN]={0}, recvmsg[BUF_LEN]={0};
	
	
	if( globalArgs.client_mode == 1 ){
		do{
			//ret = fread(sendmsg, sizeof(char), BUF_LEN, stdin);
			//fgets(sendmsg, BUF_LEN, stdio);
			memset(sendmsg, 0, sizeof(sendmsg));
			memset(recvmsg, 0, sizeof(recvmsg));
			scanf("%s", sendmsg);
			fprintf(stdout, "%s", sendmsg);
			ret2 = sendto(comm_sock_id, sendmsg, strlen(sendmsg), 0, (struct sockaddr *)&globalArgs.sock_addr, sizeof(struct sockaddr));
			if(ret2 <0){
				fprintf(stderr, "%sline%d: Failed to sendto() socket\r\n", __FUNCTION__, __LINE__);	
				return -2;
			}
			while (1) {
				addr_len = sizeof(struct sockaddr);
				if ( ret = recvfrom (comm_sock_id, recvmsg, BUF_LEN, 0, (struct sockaddr *)&globalArgs.sock_addr, &addr_len) <= 0 ) {
					continue;
				}
				printf("ret=%d, recv from %s[:%d] : %s\n", ret, inet_ntoa(globalArgs.sock_addr.sin_addr), ntohs(globalArgs.sock_addr.sin_port), recvmsg?recvmsg:"Null receive");
				break;
			}
		}while(1);
	}else{
		assert( globalArgs.server_mode==1 );
		do{
			//memset(sendmsg, 0, sizeof(sendmsg));
			memset(recvmsg, 0, sizeof(recvmsg));
			while (1) {
				addr_len = sizeof(struct sockaddr);
				if ( ret = recvfrom (comm_sock_id, recvmsg, BUF_LEN, 0, (struct sockaddr *)&globalArgs.client_addr, &addr_len) <= 0 ) {
					continue;
				}
				printf("ret=%d, recv from %s[:%d] : %s\n", ret, inet_ntoa(globalArgs.client_addr.sin_addr), ntohs(globalArgs.client_addr.sin_port), recvmsg?recvmsg:"Null receive");
				break;
			}
			handle_data( recvmsg );
			//ret2 = sendto(comm_sock_id, recvmsg, ret, 0, (struct sockaddr *)&globalArgs.client_addr, sizeof(struct sockaddr));
			ret2 = sendto(comm_sock_id, recvmsg, strlen(recvmsg), 0, (struct sockaddr *)&globalArgs.client_addr, sizeof(struct sockaddr));
			if(ret2 <=0){
				fprintf(stderr, "%sline%d: Failed to sendto() socket\r\n", __FUNCTION__, __LINE__);	
				return -1;
			}
			
		}while(1);
		
	}
	
	return 0;
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
	
	ret = data_hub( comm_sock_id );
	dbg("End of data hub, what's matter? ret:%d\n", ret);
	
	return 0;       
	
	err2:
		//fclose(fp);
	err1:
		free (buf);			
		return -2;
}

                   