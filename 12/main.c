/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 12

   Topic: sorting

   file name: main.c

   Date: May 25th, 2016

   Objective: Implement four sinple sort routines, bubble, insert,
              selection, and shell, as well as three advanced sort
	      routines, quick, heap, and merge sorts.  Get the array
              of input from the input file, input.txt, and ask the
              user to select one for each simple and advanced sort
              routine.  The result of the sorted array, done by
              two selected sort routine, will be displayed on the
              console, as well as saved into the output file, output.txt.

   Input array:  49, 17, 56, 85, 12, 97, 33, 71, 19, 62, 38, 84, 51,
                 29, 77, 65, 44, 99,  4, 47, 67, 41, 23, 88, 73,  8,
                100, 25, 91, 58, 59, 22, 15, 35, 95, 60, 20,  7, 50,
                 10
              */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static int xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	if (os != stdout) {
		va_end(ap);
		va_start(ap, fmt);
		vfprintf(os, fmt, ap);
	}
	va_end(ap);

	return ret;
}

static void prompt(FILE *os)
{
	xprintf(os, "Which simple sorting algorithm do you like or (Q) for quit?\n");
	xprintf(os, "(S)election, (I)nsertion? (B)ubble, or s(H)ell? ");
}

static int input(FILE *os)
{
	char answer, nl;
	int ret;

	ret = scanf("%c%c", &answer, &nl);
	if (ret != 2)
		return EOF;

	switch (tolower(answer)) {
		case 's':
		case 'i':
		case 'b':
		case 'h':
			return 1;
		case 'q':
			return EOF;
		default:
			return 0;
	}
}

static void process(FILE *is, FILE *os)
{
	for (prompt(os); input(os) != EOF; prompt(os))
		fprintf(os, "Hello world\n");
}

int main()
{
	const char *input_file = "input.txt";
	const char *output_file = "output.txt";
	FILE *is = NULL, *os = NULL;
	int ret = EXIT_SUCCESS;

	is = fopen(input_file, "r");
	if (is == NULL) {
		fprintf(stderr, "can't open '%s' for reading.\n",
				input_file);
		ret = EXIT_FAILURE;
		goto err;
	}

	os = fopen(output_file, "w");
	if (os == NULL) {
		fprintf(stderr, "can't open '%s' for writing.\n",
				output_file);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Let's light my fire! */
	process(is, os);

err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
