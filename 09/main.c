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
#include <stdbool.h>
#include <ctype.h>

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

/* Array based queue. */
struct aqueue {
	char begin;
	char end;
	char size;
	char capacity;
#define MAX_SIZE 3
	int array[MAX_SIZE];
};

static void init(struct aqueue *q)
{
	q->begin = q->end = q->size = 0;
	q->capacity = sizeof(q->array) / sizeof(int);
}

static bool is_empty(const struct aqueue *q)
{
	return q->size == 0;
}

static bool is_full(const struct aqueue *q)
{
	return q->size == q->capacity;
}

static int print_queue(const struct aqueue *q, FILE *os)
{
	int counter = 0;

	if (!is_empty(q)) {
		char end = q->begin < q->end ? q->end : q->capacity;
		int i;

		for (i = q->begin; i < end; ++i, ++counter)
			xprintf(os, "%c", q->array[i]);

		/* Queue is wrapped around. */
		if (q->end < q->begin)
			for (i = 0; i < q->end; ++i, ++counter)
				xprintf(os, "%c ", q->array[i]);
	}
	return counter;
}

static void process(FILE *is, FILE *os)
{
	struct aqueue queue;
	int ret;

	xprintf(os, "\nEnque and Deque test with the simple array queue\n");
	xprintf(os, "================================================\n\n");

	init(&queue);
	while ((ret = fgetc(is)) != EOF)
		/* Ignore white space. */
		if (isalnum(ret))
			xprintf(os, "%c", ret);
	xprintf(os, "\n");

	xprintf(os, "\nFinal queue contents: ");
	if (print_queue(&queue, os) == 0)
		xprintf(os, "Queue is empty\n");

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
