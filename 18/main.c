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

static const int max_number_of_numbers = 2;
static const int reading_unit_size = 4;
#define DEBUG
#ifdef DEBUG
static const char separator = ',';
#else
static const char separator = ' ';
#endif /* DEBUG */

struct number {
	int value;
	int position;
	struct number *next;
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

static struct number *__new_number(const int value)
{
	struct number *number;

	number = malloc(sizeof(struct number));
	assert(number != NULL);
	number->next = NULL;
	number->value = value;
	number->position = 0;

	return number;
}

static struct number *new_number(const char *const num)
{
	int value;
	int ret;

	ret = sscanf(num, "%d", &value);
	assert(ret == 1);

	return __new_number(value);
}

static void free_number(struct number *number)
{
	assert(number->next == NULL);
	free(number);
}

static struct number *push_number(struct number *head,
		struct number *const number)
{
	if (head != NULL)
		number->position = head->position + 1;
	else
		number->position = 0;
	number->next = head;
	return number;
}

static struct number *pop_number(struct number **head)
{
	struct number *number = *head;

	if (number != NULL) {
		*head = number->next;
		number->next = NULL;
	}
	return number;
}

static int __print_number(FILE *os, const struct number *const number,
		const bool is_head)
{
	int ret = 0;

	if (is_head) {
		if ((number->value / 10) == 0)
			ret += xprintf(os, "   ");
		else if ((number->value / 100) == 0)
			ret += xprintf(os, "  ");
		else if ((number->value / 1000) == 0)
			ret += xprintf(os, " ");
		ret += xprintf(os, "%d", number->value);
	} else
		ret += xprintf(os, "%04d", number->value);

	if (number->next != NULL)
		ret += xprintf(os, "%c", separator);

	return ret;
}

static struct number *alloc_number(char *buf)
{
	size_t size = strlen(buf);
	struct number *head = NULL;
	char *ptr;

	/* Get every four digit, from the least significat part.  */
	for (ptr = buf + size - reading_unit_size; ptr >= buf;
			ptr -= reading_unit_size) {
		head = push_number(head, new_number(ptr));
		*ptr = '\0';
	}

	/* Most significant part. */
	if (ptr + reading_unit_size > buf)
		head = push_number(head, new_number(buf));

	return head;
}

static void delete_number(struct number *head)
{
	struct number *number;

	while ((number = pop_number(&head)))
		free_number(number);
}

static struct number *__copy_number(const struct number *const from)
{
	struct number *to;

	to = __new_number(from->value);
	to->position = from->position;

	return to;
}

static struct number *copy_number(const struct number *const from_head)
{
	const struct number *from = from_head;
	struct number *head, *prev;

	head = prev = __copy_number(from);
	while ((from = from->next)) {
		prev->next = __new_number(from->value);
		prev->next->position = from->position;
		prev = prev->next;
	}
	return head;
}

static struct number *__add_number(struct number *to_head,
		const struct number *const from_head)
{
	int max_position = to_head->position > from_head->position ?
		to_head->position : from_head->position;
	const struct number *from = from_head;
	struct number *to = to_head;
	struct number *head, *prev;
	int i;

	head = prev = to;
	for (i = max_position; i >= 0; i--) {
		if (to->position == from->position) {
			to->value += from->value;
			if (to->value >= 10000) {
				prev->value += to->value / 10000;
				to->value %= 10000;
			}
			prev = to;
			from = from->next;
			to = to->next;
		} else if (from->position == i) {
			if (head == to) {
				head = prev = __copy_number(from);
			} else {
				prev->next = __copy_number(from);
				prev = prev->next;
			}
			prev->next = to;
			from = from->next;
		} else if (to->position == i)
			to = to->next;
	}

	return to_head;
}

static struct number *add_number(struct number *to,
				const struct number *const from)
{
	if (to == NULL)
		return copy_number(from);
	else
		return __add_number(to, from);
}

static int print_number(FILE *os, const struct number *const head,
		const size_t max_position)
{
	const struct number *number;
	bool is_head = true;
	int offset;
	int ret = 0;

	for (offset = max_position - head->position; offset > 0; offset--)
		xprintf(os, "%5c", ' ');

	for (number = head; number != NULL; number = number->next) {
		ret += __print_number(os, number, is_head);
		is_head = false;
	}

	return ret;
}

static int get_numbers(FILE *is, struct number *nums[], const size_t size,
		int *max_position)
{
	struct number *num;
	char buf[BUFSIZ];
	int i = 0;

	*max_position = 0;
	while (fscanf(is, "%s\n", buf) != EOF) {
		num = nums[i++] = alloc_number(buf);
		if (*max_position < num->position)
			*max_position = num->position;
		if (i == size)
			break;
	}
	return i;
}

static int print_title(FILE *os)
{
	int ret;

	ret = xprintf(os, "\nBig number arithmatic\n");
	ret += xprintf(os, "=====================\n\n");

	return ret;
}

static void process(FILE *is, FILE *os)
{
	struct number *numbers[max_number_of_numbers];
	struct number *result = NULL;
	int max_position;
	int i, num;

	print_title(os);

	num = get_numbers(is, numbers, max_number_of_numbers,
			&max_position);

	for (i = 0; i < num; i++) {
		if (i + 1 < num)
			xprintf(os, "  ");
		else
			xprintf(os, "+ ");

		print_number(os, numbers[i], max_position);
		result = add_number(result, numbers[i]);
		xprintf(os, "\n");
	}

	xprintf(os, "--");
	for (i = 0; i <= max_position; i++)
		xprintf(os, "-----");
	xprintf(os, "\n  ");
	print_number(os, result, max_position);
	xprintf(os, "\n");

	for (i = 0; i < num; i++)
		delete_number(numbers[i]);
	delete_number(result);
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
