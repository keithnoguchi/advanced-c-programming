/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 9

   Topic: Enque and Deque

   file name: main.c

   Date: May 17th, 2016

   Objective:
*/

#include <stdio.h>

static void process(FILE *is, FILE *os)
{
	int ret;

	while ((ret = fgetc(is)) != EOF)
		fputc(ret, os);
}

int main()
{
	const char *input_filename = "input.txt";
	const char *output_filename = "output.txt";
	FILE *is = NULL, *os = NULL;
	int ret;

	is = fopen(input_filename, "r");
	if (is == NULL) {
		fprintf(stderr, "Can't open '%s' file for reading.\n",
				input_filename);
		goto err;
	}

	os = fopen(output_filename, "w");
	if (os == NULL) {
		fprintf(stderr, "Can't open '%s' file for writing.\n",
				output_filename);
		goto err;
	}

	/* Light the fire! */
	process(is, os);

err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);
}

