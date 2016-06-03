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

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
	BUBBLE_SORT,
	INSERTION_SORT,
	SELECTION_SORT,
	SHELL_SORT,
	QUICK_SORT,
	HEAP_SORT,
	MERGE_SORT,
	SORT_MAX,
	SORT_UNKNOWN = SORT_MAX,
	SORT_QUIT
} sort_t;

struct list {
	size_t size;
	size_t max;
	size_t alloc;
	size_t step;
	int *data;
};

struct sorter {
	sort_t type;
	char *name;
	void (*func)(struct list *l);
};

typedef void (*prompt_func_t)(FILE *os);
typedef sort_t (*input_func_t)(FILE *os);

#define INITIAL_MAX_SIZE 10
const size_t max_size = INITIAL_MAX_SIZE;

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

static int value(const struct list *l, const size_t i)
{
	return i < l->size ? l->data[i] : -1;
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

	xprintf(os, "\n");
	for (i = 0; i < l->size; ++i)
		xprintf(os, "%d, ", l->data[i]);
	xprintf(os, "\n");
}

static void copy(const struct list *src, struct list *dst)
{
	if (dst->alloc < src->alloc) {
		dst->data = realloc(dst->data, src->alloc);
		if (dst->data == NULL)
			return;
	}
	dst->size = src->size;
	dst->max = src->max;
	dst->alloc = src->alloc;
	dst->step = src->step;
	memcpy(dst->data, src->data, dst->alloc);
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
		return SORT_UNKNOWN;

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
			return SORT_UNKNOWN;
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
		return SORT_UNKNOWN;

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
			return SORT_UNKNOWN;
	}
}

static sort_t selection(FILE *os, const prompt_func_t prompt,
		const input_func_t input)
{
	sort_t ret;

	for ((*prompt)(os); (ret = (*input)(os)) != SORT_QUIT;
			(*prompt)(os))
		if (ret != SORT_UNKNOWN)
			break;

	return ret;
}

static void selection_sort(struct list *l)
{
	int i, j;

	for (i = 0; i < l->size - 1; ++i)
		for (j = i + 1; j < l->size; ++j)
			if (value(l, j) < value(l, i))
				swap(l, i, j);
}

static void __quick_sort(struct list *l, const size_t low, const size_t high)
{
	size_t i, last;

	/* Base case. */
	if (low + 1 >= high)
		return;

	/* Put pivot on slot low. */
	swap(l, low, rand() % (high - low) + low);

	/* Partitioning. */
	last = low;
	for (i = low + 1; i < high; ++i)
		if (value(l, i) < value(l, low))
			swap(l, i, ++last);
	swap(l, low, last);

	__quick_sort(l, low, last);
	__quick_sort(l, last, high);
}

static void quick_sort(struct list *l)
{
	/* For rand() for pivot. */
	srand(time(NULL));

	__quick_sort(l, 0, l->size);
}

static void merge(int data[], const size_t low, const size_t mid,
		const size_t high, int work[])
{
	int i = low, j = mid;
	int k = low, l;

	while (i < mid && j < high) {
		if (data[i] <= data[j])
			work[k++] = data[i++];
		else
			work[k++] = data[j++];
	}

	/* Take care of the rest of the data. */
	l = i == mid ? j : i;
	while (k < high)
		work[k++] = data[l++];

	/* Copy back the result to the original arary. */
	for (i = low; i < high; ++i)
		data[i] = work[i];
}

static void __merge_sort(int data[], const size_t low, const size_t high,
		int work[])
{
	size_t mid;

	/* Base case. */
	if (low + 1 >= high)
		return;

	mid = (low + high) / 2;
	__merge_sort(data, low, mid, work);
	__merge_sort(data, mid, high, work);

	merge(data, low, mid, high, work);
}

static void merge_sort(struct list *l)
{
	struct list work;

	init(&work, max_size);
	copy(l, &work);

	/* Let's do the work recursively. */
	__merge_sort(l->data, 0, l->size, work.data);

	term(&work);
}

static const struct sorter sorters[SORT_MAX] = {
	{
		.type = BUBBLE_SORT,
		.name = "Bubble sort",
		.func = NULL
	},
	{
		.type = INSERTION_SORT,
		.name = "Insertion sort",
		.func = NULL
	},
	{
		.type = SELECTION_SORT,
		.name = "Selection sort",
		.func = selection_sort
	},
	{
		.type = SHELL_SORT,
		.name = "Shell sort",
		.func = NULL
	},
	{
		.type = QUICK_SORT,
		.name = "Quick sort",
		.func = quick_sort
	},
	{
		.type = HEAP_SORT,
		.name = "Heap sort",
		.func = NULL
	},
	{
		.type = MERGE_SORT,
		.name = "Merge sort",
		.func = merge_sort
	}
};

static void sort(FILE *os, struct list *l)
{
	struct list temp;
	sort_t type;

	init(&temp, max_size);

	type = selection(os, prompt_simple, input_simple);
	if (type == SORT_QUIT)
		goto end;

	if (sorters[type].func == NULL) {
		xprintf(os, "\n%s sorting function has not implemented\n",
				sorters[type].name);
	} else {
		copy(l, &temp);
		xprintf(os, "\nResult of the %s\n", sorters[type].name);
		(*sorters[type].func)(&temp);
		print(os, &temp);
	}

	type = selection(os, prompt_advanced, input_advanced);
	if (type == SORT_QUIT)
		goto end;

	if (sorters[type].func == NULL) {
		xprintf(os, "\n%s sorting function has not implemented\n",
				sorters[type].name);
	} else {
		copy(l, &temp);
		xprintf(os, "\nResult of the %s\n", sorters[type].name);
		(*sorters[type].func)(&temp);
		print(os, &temp);
	}

end:
	term(&temp);
}

int main()
{
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

	xprintf(os, "\nSimple and advanced sorting, in ascending order\n");
	xprintf(os, "==============================================\n");

	/* Initialize the list. */
	init(&list, max_size);

	xprintf(os, "\nReading values from the '%s' file:\n", input_file);
	while (fscanf(is, "%d, ", &value) != EOF)
		add(&list, value);

	/* Print out the list. */
	print(os, &list);

	/* Sort it. */
	sort(os, &list);

	/* Cleanup the list. */
	term(&list);
err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
