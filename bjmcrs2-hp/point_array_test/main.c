#include <stdlib.h>
char *a;
void insert_a();

int main(void) 
{
	a = malloc(100);
	if (a == NULL) {
		/* Handle malloc() error */
		printf("Fail to malloc\n");
		return -1;
	}
	insert_a();
	return 0;
}
