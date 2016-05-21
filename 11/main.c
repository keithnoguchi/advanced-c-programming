/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 11

   Topic: Linked list

   file name: main.c

   Date: May 21th, 2016

   Objective: Create a linked-list based on the input from a file.
              Do the following operations against the created
              linked-list:

              1) Reverse the list
              2) Delete the node, identified by the user

              Print the list after removing elements.

              Here is the sample input numbers:

              49, 17, 56, 85, 12, 97, 33, 71, 19, 62, 38, 84, 51,
              29, 77, 65, 44, 99, 4, 47, 67, 41, 23, 88, 73, 8, 10,
              100, 25, 91, 58, 45, 22, 15, 35, 95, 60, 20, 7, 50

              Here is the sample input by the user for deletion:

              71, 51, 38, 5, 0, 25, 42, 32 and 47
*/

#include <stdio.h>

int main()
{
	const char *input_file = "input.txt";
	const char *output_file = "output.txt";
	FILE *is, *os = NULL;

	is = fopen(input_file, "r");
	if (is == NULL) {
		fprintf(stderr, "Can't open '%s' file to read.\n",
				input_file);
		goto err;
	}

	os = fopen(output_file, "w");
	if (os == NULL) {
		fprintf(stderr, "Can't open '%s' file to write.\n",
				output_file);
		goto err;
	}

err:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);
}
