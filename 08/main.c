/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 8

   Topic: Check the scoping validity

   file name: main.c

   Date: May 15th, 2016

   Objective: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum scope_symbol_type {
	OPEN_PARENTHESIS = 0,
	OPEN_BRACE,
	OPEN_BRACKET,
	OPEN_SYMBOL_MAX,
	CLOSE_PARENTHESIS = OPEN_SYMBOL_MAX,
	CLOSE_BRACE,
	CLOSE_BRACKET,
	CLOSE_SYMBOL_MAX,
	SYMBOL_MAX = CLOSE_SYMBOL_MAX,
	NOT_A_SYMBOL = SYMBOL_MAX
} scope_symbol_t;

static const char scope_symbols[SYMBOL_MAX] = {
	'(', '{', '[', ')', '}', ']'
};

static int xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vfprintf(stdout, fmt, ap);
	va_end(ap);
	if (os != stdout && os != stderr) {
		va_start(ap, fmt);
		ret = vfprintf(os, fmt, ap);
		va_end(ap);
	}
	return ret;
}

static const scope_symbol_t scope_symbol_type(const char c)
{
	scope_symbol_t type;

	for (type = OPEN_PARENTHESIS; type < SYMBOL_MAX; ++type)
		if (c == scope_symbols[type])
			return type;

	return NOT_A_SYMBOL;
}

static bool is_scope_symbol(const char c)
{
	return scope_symbol_type(c) != NOT_A_SYMBOL;
}

static bool is_open_symbol(const char c)
{
	return scope_symbol_type(c) < OPEN_SYMBOL_MAX;
}

static bool is_close_symbol(const char c)
{
	return is_scope_symbol(c) && !is_open_symbol(c);
}

static void push(FILE *os, const char c)
{
	xprintf(os, "PUSH %c\n", c);
}

static char pop(FILE *os, const char c)
{
	xprintf(os, "POP %c\n", c);
}

static void validate_scopes(FILE *is, FILE *os)
{
	int c;

	while ((c = fgetc(is)) != EOF) {
		if (is_open_symbol(c))
			push(os, c);
		else if (is_close_symbol(c))
			pop(os, c);
	}
}

int main()
{
	const char *out_filename = "output.txt";
	const char *in_filename = "input.txt";
	FILE *os, *is;
	int ret = EXIT_SUCCESS;

	/* Output stream. */
	os = fopen(out_filename, "w");
	if (os == NULL) {
		fprintf(stderr, "can't open %s\n", out_filename);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Input stream. */
	is = fopen(in_filename, "r");
	if (is == NULL) {
		fprintf(stderr, "can't open %s\n", in_filename);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Validate the scoping of the equation. */
	validate_scopes(is, os);

err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
