/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 7

   Topic: Eight queens

   file name: main.c

   Date: May 12nd, 2016

   Objective:

*/

#include <stdio.h>
#include <stdbool.h>

int main()
{
	char c, nl;
	int ret;

	printf("Eight queen problem\n");
	while (true) {
		printf("Input something: ");
		ret = scanf("%c%c", &c, &nl);
		if (ret == EOF)
			break;
		printf("%c%c", c, nl);
	}
	printf("\nThank you!\n");
}
