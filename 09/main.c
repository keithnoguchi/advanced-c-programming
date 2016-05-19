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
#define MAX_SIZE 6
	int data[MAX_SIZE];
};

static void init(struct aqueue *q)
{
	q->begin = q->end = q->size = 0;
	q->capacity = sizeof(q->data) / sizeof(int);
}

static bool is_empty(const struct aqueue *q)
{
	return q->size == 0;
}

static bool is_full(const struct aqueue *q)
{
	return q->size == q->capacity;
}

static char next(const struct aqueue *q, const char current)
{
	/* Wrap around handling. */
	return current + 1 == q->capacity ? 0 : current + 1;
}

static int enque(struct aqueue *q, int value)
{
	q->data[q->end] = value;
	q->end = next(q, q->end);
	/* Adjust the begin position, in case of overflow. */
	if (is_full(q))
		q->begin = next(q, q->begin);
	else
		++q->size;
}

static int deque(struct aqueue *q)
{
	if (is_empty(q))
		return EOF;
	else {
		int ret;

		ret = q->data[q->begin];
		q->begin = next(q, q->begin);
		--q->size;
		return ret;
	}
}

static int print(const struct aqueue *q, FILE *os)
{

	if (is_empty(q))
		xprintf(os, "Queue is empty");
	else {
		int remain, i = q->begin;

		for (remain = q->size; remain > 0; --remain) {
			xprintf(os, "%d ", q->data[i]);
			i = next(q, i);
		}
	}
}

static void process(FILE *is, FILE *os)
{
	struct aqueue queue;
	int ret;
	int i;

	xprintf(os, "\nEnque and Deque test with the simple array queue\n");
	xprintf(os, "================================================\n\n");

	init(&queue);
	i = 0;
	while ((ret = fgetc(is)) != EOF) {
		/* First get the queue operator. */
		if (isalpha(ret)) {
			xprintf(os, "%2d) ", ++i);
			if (tolower(ret) == 'e') {
				/* Enque operation. */
				fscanf(is, "%d", &ret);
				enque(&queue, ret);
				xprintf(os, "Enque %d", ret);
			} else if (tolower(ret) == 'd') {
				/* Deque operation. */
				ret = deque(&queue);
				if (ret == EOF)
					xprintf(os, "Deque underflow");
				else
					xprintf(os, "Deque %d", ret);
			}
			xprintf(os, ", and the current queue is: ");
			print(&queue, os);
			xprintf(os, "\n");
		}
	}

	xprintf(os, "\nFinal queue contents: ");
	print(&queue, os);
	xprintf(os, "\n");

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
