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

static void process(FILE *is, FILE *os)
{
	int ret;

	xprintf(os, "\nEnque and Deque test with simple array queue\n");
	xprintf(os, "============================================\n\n");

	while ((ret = fgetc(is)) != EOF)
		xprintf(os, "%c", ret);

	xprintf(os, "\nThank you!\n");
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

