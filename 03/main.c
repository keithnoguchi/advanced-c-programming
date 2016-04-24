/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 3

   Topic: Array, reverse, sort, and indexing.

   file name: main.c

   Date: April 23rd, 2016

   Objective: Read the input into an array from standard input
              integer:

              14, 17, 56, 85, 22, 97, 33, 71, 19, 62, 38, 44, 51
	      29, 77, 65, 40, 99, 4, 47, 67, 41, 23, 81, 73, 8,
	      100, 35, 91, 58, 58, 25, 15, 38, 95, 60, 20, 7, 50,
	      10

	      Print the array in the original form.
	      Print the array in a reversed order.
	      Print the array after sorting in ascending order.
	      Identify the indices of the array for the following
	      number:
	      47, 71, 5, 58, 95, 22, 0, and 47

   Example:

*/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* debug flag. */
static const bool debug = true;

static int read_input(int a[], const size_t n)
{
	int i, value, ret;

	i = 0;
	while (i < n) {
		ret = scanf("%d,", &value);
		if (ret == EOF)
			break;
		else if (ret != 1)
			continue;
		a[i++] = value;
	}
	return i;
}

static void print(const char *msg, int a[], const size_t n)
{
	int i;

	printf("%s: ", msg);
	for (i = 0; i < n - 1; ++i)
		printf("%d, ", a[i]);
	printf("%d\n", a[i]);
}

static void copy(int dst[], const int src[], const size_t n)
{
	int i;

	for (i = 0; i < n; ++i)
		dst[i] = src[i];
}

static void swap(int a[], const int i, const int j)
{
	int tmp = a[i];

	a[i] = a[j];
	a[j] = tmp;
}

static void reverse(int a[], const size_t n)
{
	int i, j;

	/* Swap a[i] and a[j] until i crosses j. */
	for (i = 0, j = n - 1; i < j; ++i, --j)
		swap(a, i, j);
}

static int compare(const int a[], const int b[], const size_t n)
{
	int i;

	for (i = 0; i < n; ++i)
		if (a[i] != b[i])
			return a[i] - b[i];
	return 0;
}

static void move(int dst[], int src[], const size_t n)
{
	int i;

	for (i = n - 1; i >= 0; --i)
		dst[i] = src[i];
}

static void insert_sort(int a[], const size_t n)
{
	int i, j, tmp;

	for (i = 1; i < n; ++i) {
		for (j = 0; j < i; ++j) {
			if (a[j] > a[i]) {
				tmp = a[i];
				move(&a[j + 1], &a[j], i - j);
				a[j] = tmp;
			}
		}
	}
}

static void quick_sort(int a[], const size_t n)
{
	int i, last;

	/* Condition statement. */
	if (n <= 1)
		return;

	/* Put the pivot to the position 0. */
	swap(a, 0, rand() % n);
	last = 0;

	/* Find the position of the pivot. */
	for (i = 1; i < n; ++i)
		if (a[i] < a[0])
			swap(a, ++last, i);

	/* Put the pivot value to the right position. */
	swap(a, 0, last);

	/* Recursively sort both parts.  */
	quick_sort(a, last);
	quick_sort(a + last + 1, n - last - 1);
}

static void sort(int a[], const size_t n, void (*fn)(int a[], const size_t n))
{
	/* This srand() call is for quick_sort(). */
	srand(time(NULL));
	(*fn)(a, n);
}

static void prompt(void)
{
	printf("Which number are you interested? ");
}

static int get_number(void)
{
	int number;
	int ret;

	printf("get_number() in\n");
	while ((ret = scanf("%d", &number)) != 1) {
		if (ret == EOF)
			return EOF;
		printf("Just input the number, please.");
	}
	printf("get_number() out with ret = %d, number = %d\n", ret, number);
	return number;
}

static int lookup(const int a[], const int number)
{
	return -1;
}

static size_t read_it(int a[], const int size)
{
	int n;

	n = read_input(a, size);
	if (n <= 0) {
		fprintf(stderr, "Please input numbers");
		exit(-1);
	}
	print("original", a, n);

	return n;
}

static void reverse_it(const int a[], const int n)
{
	int tmp[n];

	copy(tmp, a, n);
	reverse(tmp, n);
	print("reversed", tmp, n);
	if (debug) {
		reverse(tmp, n);
		if (compare(a, tmp, n)) {
			fprintf(stderr, "Something wrong in reverse_array()\n");
			exit(-1);
		}
	}
}

static void sort_it(const int a[], const size_t n)
{
	int tmp[n];

	copy(tmp, a, n);
	sort(tmp, n, quick_sort);
	print("sorted(quick_sort)", tmp, n);
	if (debug) {
		int debug_tmp[n];
		copy(debug_tmp, a, n);
		sort(debug_tmp, n, insert_sort);
		if (compare(tmp, debug_tmp, n)) {
			fprintf(stderr, "Something wrong with quick_sort()\n");
			exit(-1);
		}
	}
}

static void lookup_it(const int a[], const size_t n)
{
	int number;
	int index;

	for (prompt(); (number = get_number()) != EOF; prompt()) {
		index = lookup(a, number);
		if (index == -1)
			printf("There is no such number\n");
		else
			printf("%d is at index %d\n", number, index);
	}
	printf("\n");
}

int main()
{
	static const size_t size = 100;
	int array[size];
	int n;

	/* Read the input. */
	n = read_it(array, size);

	/* Reverse the array. */
	reverse_it(array, n);

	/* Sort the array. */
	sort_it(array, n);

	/* Identify the indices of the number. */
	lookup_it(array, n);
}
