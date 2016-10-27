#include <stdio.h>

typedef unsigned int UINT;
typedef unsigned char UCHAR;

typedef struct {
	int a;
	char b;
}T_S;

typedef struct {
	int a2;
	char b2;
}T_S2;

typedef struct {
	UINT enable:1;
	UINT err:1;
	UINT motorSpeed:4;
	UINT led:1;
}status_bits;

typedef struct {
	UCHAR enable:1;
	UCHAR err:1;
	UCHAR motorSpeed:4;
	UCHAR led:1;
}status2_bits;

typedef struct {
	UCHAR enable:1;
	UCHAR err:2;
	UCHAR motorSpeed:4;
	UCHAR led:1;
}status3_bits;

typedef struct {
	UCHAR enable:1;
	UCHAR err:2;
	UCHAR motorSpeed:4;
	UCHAR led:2;
}status4_bits;

int func(int fd, T_S *s, T_S2 *v)
{
	printf("v = %p\n", v);
	return 0;
}

int main(int argc, char **argv)
{
         T_S s = { 1, 2}, *sp;
         T_S2 s2 = { 3, 4}, *sp2;
         int fd = 10;
         
         printf("sizeof(status_bits) = %d\n", sizeof(status_bits));
         printf("sizeof(status2_bits) = %d\n", sizeof(status2_bits));         
         printf("sizeof(status3_bits) = %d\n", sizeof(status3_bits));
         printf("sizeof(status4_bits) = %d\n", sizeof(status4_bits));
         
         sp = &s;
         sp2 = &s2;
         printf("sp2 = %p\n", sp2);
         func(fd, sp, sp2);
         printf("sp2 = %p\n", sp2);
         
         return 0;
}