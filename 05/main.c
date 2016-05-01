/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 5

   Topic: Pointers and pointer of pointers.

   file name: main.c

   Date: April 30th, 2016

   Objective: There is an integer 25, a float 3.142, a double 1.73217,
   a string "Hello, C Programmer", and a char 'x'.

*/

#include <stdio.h>

int main()
{
	int i = 25;
	float f = 3.142;
	double d = 1.73217;
	char *s = "Hello, C Programmer";
	char c = 'x';

	printf("Print out the value itself\n");
	printf("--------------------------\n");
	printf("%-15s %d\n", "Integer i:", i);
	printf("%-15s %f\n", "Float f:", f);
	printf("%-15s %F\n", "Double d:", d);
	printf("%-15s %s\n", "String s:", s);
	printf("%-15s %c\n", "Character c:", c);
}
