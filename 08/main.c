/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 8

   Topic: Check the scoping validity

   file name: main.c

   Date: May 15th, 2016

   Objective: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static int xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vfprintf(stdout, fmt, ap);
	va_end(ap);
	if (os != stdout && os != stderr) {
		va_start(ap, fmt);
		ret = vfprintf(os, fmt, ap);
		va_end(ap);
	}

	return ret;
}

int main()
{
	const char *out_filename = "output.txt";
	const char *in_filename = "input.txt";
	FILE *os, *is;
	int ret = EXIT_SUCCESS;
	int c;

	/* Output stream. */
	os = fopen(out_filename, "w");
	if (os == NULL) {
		fprintf(stderr, "can't open %s\n", out_filename);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Input stream. */
	is = fopen(in_filename, "r");
	if (is == NULL) {
		fprintf(stderr, "can't open %s\n", in_filename);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Just copy the contents. */
	while ((c = fgetc(is)) != EOF)
		xprintf(os, "%c", c);

err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
