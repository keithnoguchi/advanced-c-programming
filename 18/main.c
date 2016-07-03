/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 18

   Topic: Big integer plus operation

   file name: main.c

   Date: July 2nd, 2016

   Objective: Read the following two long integer inputs from
              an input file in the same order. (Note: Do not
              assign an array with input values.)

              List 1: 954680325257568512973371196238845129776544789456

              List 2: 64324564257936835497210698463523456234

              Using a list structure read the numbers and forms
              a linear lined list, print list elements as list1
              and list2.  Consider reading 4 or 5 digits at a time
              into the node.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

static const int max_numbers = 2;

struct node {
	int value;
	struct node *next;
};

struct number {
	int size;
	struct node *head;
};

static int xprintf(FILE *os, const char *const fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vfprintf(os, fmt, ap);
	if (os != stdout && os != stderr) {
		va_end(ap);
		va_start(ap, fmt);
		ret = vprintf(fmt, ap);
	}
	va_end(ap);

	return ret;
}

static struct node *new_node(const char *const num)
{
	struct node *node;
	int ret;

	node = malloc(sizeof(struct node));
	assert(node != NULL);
	node->next = NULL;

	ret = sscanf(num, "%d", &node->value);
	assert(ret == 1);

	return node;
}

static void free_node(struct node *node)
{
	assert(node->next == NULL);
	free(node);
}

static struct node *push_node(struct node *head, struct node *const node)
{
	node->next = head;
	return node;
}

static struct node *pop_node(struct node **head)
{
	struct node *node = *head;

	if (node != NULL) {
		*head = node->next;
		node->next = NULL;
	}
	return node;
}

static int print_node(FILE *os, const struct node *const node,
		const bool is_head)
{
	if (is_head)
		return xprintf(os, "%4d ", node->value);
	else
		return xprintf(os, "%04d ", node->value);
}

static struct number *new_number(char *buf)
{
	size_t size = strlen(buf);
	struct number *num;
	char *ptr;

	num = malloc(sizeof(struct number));
	assert(num != NULL);
	num->size = 0;

	/* Get every four digit, from the botton.  */
	for (ptr = buf + size - 4; ptr >= buf; ptr -= 4) {
		num->head = push_node(num->head, new_node(ptr));
		num->size++;
		*ptr = '\0';
	}

	/* Mist significant part. */
	if (ptr + 4 > buf) {
		num->head = push_node(num->head, new_node(buf));
		num->size++;
	}

	return num;
}

static void free_number(struct number *num)
{
	assert(num->head == NULL);
	assert(num->size == 0);
	free(num);
}

static void delete_number(struct number *num)
{
	struct node *node;

	while ((node = pop_node(&num->head))) {
		free_node(node);
		num->size--;
	}

	free_number(num);
}

static int print_number(FILE *os, const struct number *const num)
{
	struct node *node;
	bool is_head = true;
	int ret = 0;

	for (node = num->head; node != NULL; node = node->next) {
		ret += print_node(os, node, is_head);
		is_head = false;
	}
	xprintf(os, "\n");

	return ret;
}

static int get_numbers(FILE *is, struct number *nums[], const size_t size)
{
	char buf[BUFSIZ];
	int i = 0;

	while (fscanf(is, "%s\n", buf) != EOF) {
		nums[i++] = new_number(buf);
		if (i == size)
			break;
	}

	return i;
}

static void process(FILE *is, FILE *os)
{
	struct number *numbers[max_numbers];
	int i, num;

	num = get_numbers(is, numbers, max_numbers);
	for (i = 0; i < num; i++) {
		print_number(os, numbers[i]);
		delete_number(numbers[i]);
	}
}

int main()
{
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;

	is = fopen(input, "r");
	if (is == NULL) {
		fprintf(stderr, "can't open %s to read.\n", input);
		goto err;
	}

	os = fopen(output, "w");
	if (os == NULL) {
		fprintf(stderr, "can't open %s to write.\n", output);
		goto err;
	}

	process(is, os);

err:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
