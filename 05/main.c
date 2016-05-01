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

              All the above have their pointers and pointers are assigned
              the addresses of respective variables.

              There is a special pointer which is a pointer to the integer
              pointer.

              You can initialize all the variables and pointers at compile
              time.  Print the value of the variables, their addresses and
              the address of the pointers.

*/

#include <stdio.h>

int main()
{
	int i = 25;
	float f = 3.142;
	double d = 1.73217;
	char *s = "Hello, C Programmer";
	char c = 'x';
	int *iptr = &i;
	float *fptr = &f;
	double *dptr = &d;
	char **sptr = &s;
	char *cptr = &c;
	int **iptrptr = &iptr;

	printf("Print out the value itself\n");
	printf("--------------------------\n");
	printf("%-35s %d\n", "Integer i:", i);
	printf("%-35s %f\n", "Float f:", f);
	printf("%-35s %F\n", "Double d:", d);
	printf("%-35s %s\n", "String s:", s);
	printf("%-35s %c\n", "Character c:", c);

	printf("\nPrint out the addresses of the value, using the pointers\n");
	printf("--------------------------------------------------------\n");
	printf("%-35s %p\n", "Address of integer i:", iptr);
	printf("%-35s %p\n", "Address of float f:", fptr);
	printf("%-35s %p\n", "Address of double d:", dptr);
	printf("%-35s %p\n", "Address of string s:", sptr);
	printf("%-35s %p\n", "Address of character c:", cptr);
	printf("%-35s %p\n", "Address of pointer to integer i:", iptrptr);
}
