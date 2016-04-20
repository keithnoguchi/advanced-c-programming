#include <stdio.h>

int main()
{
	int a = 1023;
	char c = 'a';
	char s[] = "Hello";
	float f = 3.14159;

	printf("a = %d\n", a);
	printf("c = %c\n", c);
	printf("s = %s\n", s);
	printf("f = %f\n", f);
	printf("a = %7d\n", a);
	printf("a = %-7d\n", a);
	printf("f = %.3f\n", f);
}
