#include < fcntl.h >
#include < stdio.h >
#include < stdlib.h >
#include < string.h >
#include < unistd.h >
#include < sys/ioctl.h >
#include < sys/types.h >
#include < linux/kd.h >

/* �趨Ĭ��ֵ */
#define DEFAULT_FREQ 440 /* �趨һ�����ʵ�Ƶ�� */
#define DEFAULT_LENGTH 200 /* 200 ΢�룬�����ĳ�������΢��Ϊ��λ��*/
#define DEFAULT_REPS 1 /* Ĭ�ϲ��ظ����� */
#define DEFAULT_DELAY 100 /* ͬ����΢��Ϊ��λ*/

/* ����һ���ṹ���洢���������*/
typedef struct {
	int freq; /* �������������Ƶ�ʣ���λΪHz */
	int length; /* �������ȣ���΢��Ϊ��λ*/
	int reps; /* �ظ��Ĵ���*/
	int delay; /* ���η����������΢��Ϊ��λ*/
} beep_parms_t;

/* ��ӡ������Ϣ���˳�*/
void usage_bail ( const char *executable_name ) {
	printf ( "Usage: \n \t%s [-f frequency] [-l length] [-r reps] [-d delay] \n ",
	executable_name );
	exit(1);
}

/ * �������в����������������£�
* "-f <��HZΪ��λ��Ƶ��ֵ >"
* "-l <�Ժ���Ϊ��λ�ķ���ʱ�� >"
* "-r <�ظ����� >"
* "-d <�Ժ���Ϊ��λ�ļ�Ъʱ�� >"
*/
void parse_command_line(char **argv, beep_parms_t *result) 
{
	char *arg0 = *(argv++);
	while ( *argv ) {
		if ( !strcmp( *argv,"-f" )) { /*Ƶ��*/
			int freq = atoi ( *( ++argv ) );
			if ( ( freq <= 0 ) ( freq > 10000 ) ) {
				fprintf ( stderr, "Bad parameter: frequency must be from 1..10000\n" );
				exit (1) ;
			} else {
				result->freq = freq;
				argv++;
			}
		} else if ( ! strcmp ( *argv, "-l" ) ) { /*ʱ��*/
			int length = atoi ( *(++argv ) );
			if (length < 0) {
				fprintf(stderr, "Bad parameter: length must be >= 0\n");
				exit(1);
			} else {
				result->length = length;
				argv++;
			}
		} else if (!strcmp(*argv, "-r")) { /*�ظ�����*/
			int reps = atoi(*(++argv));
			if (reps < 0) {
				fprintf(stderr, "Bad parameter: reps must be >= 0\n");
				exit(1);
			} else {
				result->reps = reps;			
				//Linux�����豸���ʵ����2���֣�
			����	argv++;
			}
		} else if (!strcmp(*argv, "-d")) { /* ��ʱ */
			int delay = atoi(*(++argv));
			if (delay < 0) {
				fprintf(stderr, "Bad parameter: delay must be >= 0\n");
				exit(1);
			} else {
				result->delay = delay;
				argv++;
			}
		}else {
			fprintf(stderr, "Bad parameter: %s\n", *argv);
			usage_bail(arg0);
		}
	}
}

int main(int argc, char **argv) 
{
	int console_fd;
	int i; /* ѭ�������� */
	
	/* �跢������ΪĬ��ֵ*/
	beep_parms_t parms = {DEFAULT_FREQ, DEFAULT_LENGTH, DEFAULT_REPS, DEFAULT_DELAY};
	
	/* �������������ܵĻ����·�������*/
	parse_command_line(argv, &parms);
	
	/* �򿪿���̨��ʧ�����������*/
	if ( ( console_fd = open ( "/dev/console", O_WRONLY ) ) == -1 ) {
		fprintf(stderr, "Failed to open console.\n");
		perror("open");
		exit(1);
	}
	
	/* ������ʼ������������*/
	for (i = 0; i < parms.reps; i++) {
		/* ����1190000�Ӻζ��������ö�֪*/
		int magical_fairy_number = 1190000/parms.freq;
		ioctl(console_fd, KIOCSOUND, magical_fairy_number); /* ��ʼ���� */
		usleep(1000*parms.length); /*�ȴ�... */
		ioctl(console_fd, KIOCSOUND, 0); /* ֹͣ����*/
		usleep(1000*parms.delay); /* �ȴ�... */
	} /* �ظ�����*/
	
	return EXIT_SUCCESS;
}
