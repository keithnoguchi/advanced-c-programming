#include <stdio.h>
#include <stdlib.h>

int main()
{
	char buf[BUFSIZ];
	int degree;
	char type;
	int ret;

	printf("Input the degree in the following "
			"format (<DEGREE> deg [C|F]): ");
	ret = scanf("%d deg %c", &degree, &type);
	if (ret != 2) {
		printf("Please follow the format (<DEGREE> deg [C|F])\n");
		exit(-1);
	}
	switch (type) {
	case 'F':
		return 0;
	case 'C':
		return 0;
	default:
		break; 
	}
	printf("%d and %c\n", degree, type);
	return 0;
}
