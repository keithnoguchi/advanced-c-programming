/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 15

   Topic: Hash

   file name: main.c

   Date: June 30th, 2016

   Objective: Implement a hash table of size 15.  Use the Separate
              Chaining for collisions.

              Give your input file of 100 records with keys.  Use
              the structure of {key, record, next} for the linked
              nodes.
*/

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define HASH_TABLE_SIZE 15
static const int invalid_key = -1;
static const int invalid_record = INT32_MIN;
static const int hash_table_size = HASH_TABLE_SIZE;
static const int total_number_of_random_keys = 100;
static const int random_key_max = 1000;
static const int input_file_generation_retry = 3;

static struct entry {
	const char *key;
	int record;
	struct entry *next;
} *table[HASH_TABLE_SIZE];

static void generate_random_file(const char *const output)
{
	FILE *os;
	int value;
	int i;

	srand(time(NULL));

	os = fopen(output, "w");
	if (os == NULL)
		return;

	for (i = 0; i < total_number_of_random_keys; i++) {
		value = rand() % random_key_max;
		fprintf(os, "%d ", value);
	}

	fclose(os);
}

static int xprintf(FILE *os, const char *const fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vfprintf(os, fmt, ap);
	if (os != stdout && os != stderr) {
		va_end(ap);
		va_start(ap, fmt);
		vprintf(fmt, ap);
	}
	va_end(ap);

	return ret;
}

static struct entry *new_entry(const char *const key)
{
	struct entry *e;

	e = malloc(sizeof(struct entry));
	assert(e != NULL);
	e->key = strndup(key, BUFSIZ);
	e->record = invalid_record;
	e->next = NULL;

	return e;
}

static void free_entry(struct entry *e)
{
	free((void *)e->key);
	free(e);
}

static int print_entry(FILE *os, const struct entry *const e)
{
	int ret;

	ret = xprintf(os, "{%s}", e->key);
	if (e->next)
		ret += xprintf(os, " -> ");
	else
		ret += xprintf(os, " -> NULL");

	return ret;
}

static void init_table(struct entry *table[], const size_t table_size)
{
	int i;

	for (i = 0; i < table_size; i++)
		table[i] = NULL;
}

static void print_table(FILE *os, struct entry *table[],
		const size_t table_size)
{
	const struct entry *e;
	int i;

	xprintf(os, "\nHash table, size of %2d\n", table_size);
	xprintf(os, "======================\n\n");

	for (i = 0; i < table_size; i++) {
		xprintf(os, "%2d: ", i);
		for (e = table[i]; e != NULL; e = e->next)
			print_entry(os, e);
		xprintf(os, "\n");
	}
	xprintf(os, "\n");
}

static int hash(const char *const key)
{
	const char *ptr = key;
	int total = 0;

	/* Sum up the ASCII character, as a digit
	 * and then make modulo. */
	for (ptr = key; *ptr != '\0'; ptr++)
		total += *ptr;

	return total % hash_table_size;
}

static struct entry *pop(struct entry **top)
{
	struct entry *e = *top;

	if (e != NULL) {
		*top = e->next;
		e->next = NULL;
	}
	return e;
}

static struct entry *push(struct entry *top, struct entry *e)
{
	e->next = top;
	return e;
}

static void read_entries(FILE *is, struct entry *table[],
		const size_t table_size)
{
	struct entry **ep;
	char key[BUFSIZ];

	while ((fscanf(is, "%s ", key)) != EOF) {
		ep = &table[hash(key)];
		*ep = push(*ep, new_entry(key));
	}
}

static void term_table(struct entry *table[], const size_t table_size)
{
	struct entry *e;
	int i;

	for (i = 0; i < table_size; i++)
		while ((e = pop(&table[i])))
			free_entry(e);
}

static void process(FILE *is, FILE *os)
{
	init_table(table, hash_table_size);
	read_entries(is, table, hash_table_size);
	print_table(os, table, hash_table_size);
	term_table(table, hash_table_size);
}

int main()
{
	int retry = input_file_generation_retry;
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;

	while ((is = fopen(input, "r")) == NULL) {
		/* Let's generate a random numbers. */
		generate_random_file(input);
		if (retry-- == 0)
			goto out;
	}

	os = fopen(output, "w");
	if (os == NULL)
		goto out;

	/* Let's create a hash table. */
	process(is, os);

out:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
