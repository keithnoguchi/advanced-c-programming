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
#include <string.h>
#include <assert.h>

struct node {
	int num;
	struct node *next;
};

struct number {
	int size;
	struct node *head;
};

static struct node *new_node(const char *const num)
{
	struct node *node;
	int ret;

	node = malloc(sizeof(struct node));
	assert(node != NULL);
	node->next = NULL;

	ret = sscanf(num, "%d", &node->num);
	assert(ret == 1);

	return node;
}

static void free_node(struct node *node)
{
	assert(node->next == NULL);
	free(node);
}

static struct number *new_number(char *buf)
{
	size_t size = strlen(buf);
	struct number *num;
	struct node *node;
	char *ptr;

	num = malloc(sizeof(struct number));
	assert(num != NULL);
	num->size = 0;

	/* Get every four digit, from the botton.  */
	for (ptr = buf + size - 4; ptr >= buf; ptr -= 4) {
		node = new_node(ptr);
		free_node(node);
		printf("%s\n", ptr);
		*ptr = '\0';
	}

	/* Mist significant part. */
	if (ptr + 4 > buf)
		printf("%s\n", buf);

	return num;
}

static void free_number(struct number *num)
{
	assert(num->head == NULL);
	assert(num->size == 0);
	free(num);
}

static int print_number(const struct number *const num)
{
	int ret;

	ret = printf("Here you go with number(size = %d)\n", num->size);

	return ret;
}

static void process(FILE *is, FILE *os)
{
	struct number *num;
	char buf[BUFSIZ];

	while (fscanf(is, "%s\n", buf) != EOF) {
		num = new_number(buf);
		print_number(num);
		free_number(num);
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
