/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 10

   Topic: Tower of Hanoi

   file name: main.c

   Date: May 18th, 2016

   Objective: Move 4, 5, or 6 disks from 'from' post to 'to' post,
   by using the middle 'auxiliary' post.
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

int main()
{
	const char *output_file = "output.txt";
	FILE *os;

	os = fopen(output_file, "w");
	if (os == NULL) {
		fprintf(stderr, "Can't open %s file.", output_file);
		goto err;
	}

	xprintf(os, "Tower of Hanoi\n");
	xprintf(os, "==============\n");

	xprintf(os, "\nThank you!\n");

err:
	if (os)
		fclose(os);
}
