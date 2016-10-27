#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TST_FILE "./tst.txt"

int main(int argc, char **argv)
{
	pid_t pid;
	char buf[128]={0};
	int temp, ret, fd, tmp_fd;
	char **chd_argv;
	
	if (argc == 3){
		chd_argv = argv+1;
	}
	fd = open( TST_FILE, O_CREAT|O_RDWR, 0666 );
	if( fd<0 ){
		fprintf(stderr, "fail to create/open file %s, cwd is %s\n", TST_FILE, getcwd(buf, sizeof(buf)-1) );
		return -2;
	}
	tmp_fd = dup(fd);
	pid = fork();
	if(pid == 0){
		fprintf(stdout, "Child is coming, input a string:\n");
		//dup2(stdin, temp);
		//dup2(stdout, stdin);
		
		if(dup2(fd, 1)<0){
			fprintf(stderr, "failed to dup2 %d to %d\n", stdout, fd);				
		}
			#if 0
			{
				char buf2[]="TEST TO WRITE fd\n";
				int len2;
				len2 = write(fd, buf2, sizeof(buf2));
				fprintf(stdout, "write %d to fd(%d)\n", fd);
			}
			#endif
		if (argc == 3){
			printf("%s: argv: %s, %s, %s\n", pid==0?"CHILD":"PARENT", argv[0], argv[1], argv[2]);
			printf("%s: child argv: %s, %s\n", pid==0?"CHILD":"PARENT", chd_argv[0], chd_argv[1]);
			printf("%s: chd_argv[0]=%s, basename(chd_argv[0])=%s, chd_argv[1]=%s\n", pid==0?"CHILD":"PARENT", chd_argv[0], basename(chd_argv[0]), chd_argv[1]);
			//ret = execl(chd_argv[0], basename(chd_argv[0]), chd_argv[1] );
			ret = execl(chd_argv[0], basename(chd_argv[0]), chd_argv[1], NULL );
			if(ret <0 ){
				fprintf(stderr, "failed to execl(), errno=%d\n", errno);	
				return -1;
			}
		}
	}else if( pid > 0 ){
		fprintf(stdout, "Parent is watching child(pid=%d)\n", pid);
		//sleep(1);
	}else{
		fprintf(stderr, "failed to fork(), errno=%d\n", errno);
		return -1;
	}
	scanf("%s", buf);
	fprintf(stdout, "%s (PID=%d, PPID=%d) Your input is %s\r\n", pid==0?"CHILD":"PARENT", getpid(), getppid(), buf);
	if( pid>0 ){
		dup2( fd, 1);	
	}
	fprintf(stdout, "%s (PID=%d, PPID=%d) END!!\r\n", pid==0?"CHILD":"PARENT", getpid(), getppid());

	return 0;
}