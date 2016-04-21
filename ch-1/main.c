/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 1

   Topic: Temperature conversion

   file name: main.c

   Date: April 20th, 2016

   Objective: Convert the unit of the degree from centigrade
              to farenheit, and visa versa, with the following
	      equasion:

	      F = (C temp * 9 / 5) + 32
	      C = (F temp - 32) * 5 / 9

   Example: 
   
   $ ./a.out
   Input degree in the format (<DEGREE> deg [C|F]):
   100 deg C
   212 deg F
   $ ./a.out
   Input the degree in the following format (<DEGREE> deg [C|F]):
   104 deg F
   40 deg C

 */
#include <stdio.h>

int main()
{
	int deg;
	char type;

	printf("Input degree in the format (<DEGREE> deg [C|F]): ");
	scanf("%d deg %c", &deg, &type);

	if (type == 'C') {
		/* Convert centigrade into farenheit. */
		deg = deg * 9 / 5 + 32;
		printf("%d deg F\n", deg);
	} else if (type == 'F') {
		/* Convert farenheit into centigrade. */
		deg = (deg - 32) * 5 / 9;
		printf("%d deg C\n", deg);
	} else {
		printf("Please follow '<DEGREE> deg [C|F]', as '100 deg C'\n");
	}
	return 0;
}
