/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 8

   Topic: Check the scoping validity

   file name: main.c

   Date: May 15th, 2016

   Objective: Evaluate the validity of the scopes in the following
              expression implementing stacks.  Ignore the operators
              and operand in the expression while checking the validity
              of the expression.

   Expression: {x + (y - [a + b]) * (c - [(d + e)] + k)}
                / (h - (j - (k - [l -n]))) + { u + [m * (p - q)]/r}
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

struct stack {
	int top;
	int max;
#define STACK_SIZE 10
	char array[STACK_SIZE];
};

static void init(struct stack *s)
{
	s->top = -1; /* -1 means empty stack. */
	s->max = sizeof(s->array) / sizeof(char);
}

static const bool is_empty(const struct stack *s)
{
	return s->top == -1;
}

static void push(struct stack *s, const char c, FILE *os)
{
	s->array[++s->top] = c;
}

static char pop(struct stack *s, FILE *os)
{
	return s->array[s->top--];
}

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

static bool is_scope_symbol(const scope_symbol_t symbol_type)
{
	return symbol_type != NOT_A_SYMBOL;
}

static bool is_open_symbol(const scope_symbol_t symbol_type)
{
	return symbol_type < OPEN_SYMBOL_MAX;
}

static bool does_symbol_match(const scope_symbol_t open_type,
				const scope_symbol_t close_type)
{
	return open_type == close_type - OPEN_SYMBOL_MAX;
}

static void validate_scoping(FILE *is, FILE *os)
{
	struct stack symbol_stack;
	bool valid = true;
	int c;

	xprintf(os, "\nValidate expression scoping\n");
	xprintf(os, "===========================\n\n");

	/* Initialize the stack. */
	init(&symbol_stack);
	while ((c = fgetc(is)) != EOF) {
		scope_symbol_t symbol_type = scope_symbol_type(c);
		if (!is_scope_symbol(symbol_type))
			continue;
		else if (is_open_symbol(symbol_type)) {
			xprintf(os, "Find open symbol '%c'\n", c);
			push(&symbol_stack, c, os);
		} else {
			char open_symbol = pop(&symbol_stack, os);
			scope_symbol_t open_type
				= scope_symbol_type(open_symbol);
			bool match = does_symbol_match(open_type, symbol_type);

			xprintf(os, "Find close symbol '%c'", c);
			if (match)
				xprintf(os, ", matches ");
			else {
				xprintf(os, ", doesn't match ");

				/* Push back the open symbol, */
				push(&symbol_stack, open_symbol, os);

				/* and flag as unmatched. */
				valid = false;
			}
			xprintf(os, "'%c'\n", open_symbol);
		}
	}

	/* Make sure all the open symbol has been closed correctly. */
	if (!is_empty(&symbol_stack))
		valid = false;

	/* Print out the result. */
	xprintf(os, "\nExpression does ");
	if (!valid)
		xprintf(os, "NOT ");
	xprintf(os, "have a valid scoping.\n\n");
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
	validate_scoping(is, os);

err:
	if (os)
		fclose(os);
	if (is)
		fclose(is);

	exit(ret);
}
