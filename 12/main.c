/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 12

   Topic: sorting

   file name: main.c

   Date: May 25th, 2016

   Objective: Implement four sinple sort routines, bubble, insertion,
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

typedef enum {
	SORT_NONE = 0,
	SORT_QUIT = 1,
	BUBBLE_SORT,
	INSERTION_SORT,
	SELECTION_SORT,
	QUICK_SORT,
	HEAP_SORT,
	MERGE_SORT,
	SHELL_SORT
} sort_t;

struct list {
	size_t size;
	size_t max;
	size_t alloc;
	size_t step;
	int *data;
};

typedef void (*prompt_func_t)(FILE *os);
typedef sort_t (*input_func_t)(FILE *os);
typedef void (*sort_func_t)(struct list *l);

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

static void init(struct list *l, const size_t max_size)
{
	l->data = malloc(sizeof(int) * max_size);
	if (l->data == NULL)
		return;
	l->step = 2; /* Double the size, when there is not enough memory. */
	l->size = 0;
	l->max = max_size;
	l->alloc = sizeof(int) * max_size;
}

static void add(struct list *l, const int value)
{
	if (l->size + 1 >= l->max) {
		/* Increase the size. */
		l->data = realloc(l->data, l->alloc * l->step);
		if (l->data == NULL)
			return;
		l->max *= l->step;
		l->alloc *= l->step;
	}
	l->data[l->size++] = value;
}

static void swap(struct list *l, const int i, const int j)
{
	int temp;

	/* Sanity check. */
	if (i >= l->size || j >= l->size)
		return;

	temp = l->data[i];
	l->data[i] = l->data[j];
	l->data[j] = temp;
}

static void print(FILE *os, const struct list *l)
{
	int i;

	for (i = 0; i < l->size; ++i)
		xprintf(os, "%d, ", l->data[i]);
	xprintf(os, "\n");
}

static void term(struct list *l)
{
	if (l->data)
		free(l->data);
	l->data = NULL;
	l->alloc = l->size = 0;
}

static void prompt_simple(FILE *os)
{
	xprintf(os, "\nWhich simple sort routine do you like\n");
	xprintf(os, "\n(B)ubble, (I)nsertion, (S)election, s(H)ell, or (X) for quit? ");
}

static sort_t input_simple(FILE *os)
{
	char answer, nl;
	int ret;

	ret = scanf("%c%c", &answer, &nl);
	if (ret != 2)
		return SORT_NONE;

	xprintf(os, "%c\n", answer);
	switch (tolower(answer)) {
		case 'b':
			return BUBBLE_SORT;
		case 'i':
			return INSERTION_SORT;
		case 's':
			return SELECTION_SORT;
		case 'h':
			return SHELL_SORT;
		case 'x':
			return SORT_QUIT;
		default:
			xprintf(os, "\nInvalid option '%c', plase try again.\n",
				answer);
			return SORT_NONE;
	}
}

static void prompt_advanced(FILE *os)
{
	xprintf(os, "\nWhich advanced sort routine do you like\n");
	xprintf(os, "\n(Q)uick, (H)eap, (M)erge sort, or (X) for quit? ");
}

static sort_t input_advanced(FILE *os)
{
	char answer, nl;
	int ret;

	ret = scanf("%c%c", &answer, &nl);
	if (ret != 2)
		return SORT_NONE;

	xprintf(os, "%c\n", answer);
	switch (tolower(answer)) {
		case 'q':
			return QUICK_SORT;
		case 'h':
			return HEAP_SORT;
		case 'm':
			return MERGE_SORT;
		case 'x':
			return SORT_QUIT;
		default:
			xprintf(os, "\nInvalid option '%c', plase try again.\n",
				answer);
			return SORT_NONE;
	}
}

static sort_t selection(FILE *os, const prompt_func_t prompt,
		const input_func_t input)
{
	sort_t ret;

	for ((*prompt)(os); (ret = (*input)(os)) != SORT_QUIT;
			(*prompt)(os))
		if (ret != SORT_NONE)
			break;

	return ret;
}

static void process(FILE *os, struct list *l)
{
	sort_t simple, advanced;

	simple = selection(os, prompt_simple, input_simple);
	if (simple == SORT_QUIT)
		return;

	advanced = selection(os, prompt_advanced, input_advanced);
	if (advanced == SORT_QUIT)
		return;

	xprintf(os,
		"\nSimple sort routine is %d and advanced sort routine is %d\n",
		simple, advanced);
}

int main()
{
#define INITIAL_MAX_SIZE 10
	const size_t max_size = INITIAL_MAX_SIZE;
	const char *input_file = "input.txt";
	const char *output_file = "output.txt";
	FILE *is = NULL, *os = NULL;
	int ret = EXIT_SUCCESS;
	struct list list;
	int value;

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

	xprintf(os, "\nSimple and advanced sorting\n");
	xprintf(os, "===========================\n");

	/* Initialize the list. */
	init(&list, max_size);

	xprintf(os, "\nReading values from the '%s' file:\n\n", input_file);
	while (fscanf(is, "%d, ", &value) != EOF)
		add(&list, value);

	/* Print out the list. */
	print(os, &list);

	/* Sort it. */
	process(os, &list);

	/* Cleanup the list. */
	term(&list);
err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
