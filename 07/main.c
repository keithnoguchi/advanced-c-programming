/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 7

   Topic: Eight queens

   file name: main.c

   Date: May 14th, 2016

   Objective: Place 8 queens on an 8 x 8 chess board so that none of the
              queens attack each other.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

#define max_row 8
#define max_column 8

/* Final board positions.  */
static char board[max_row][max_column];

/* Keep track of the free positions. */
static bool free_column[max_column];
static bool free_up[max_row + max_column - 1];
static bool free_down[max_row + max_column - 1];

static void xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);

	if (os != stdout) {
		va_start(ap, fmt);
		vfprintf(os, fmt, ap);
		va_end(ap);
	}
}

static bool is_free(const int column, const int row)
{
	if (!free_column[column])
		return false;
	else if (!free_up[column + row])
		return false;
	else if (!free_down[max_column - 1 - column + row])
		return false;
	else
		return true;
}

static void update_free_slots(const int column, const int row,
		              const bool state)
{
	free_column[column] = state;
	free_up[column + row] = state;
	free_down[max_column - 1 - column + row] = state;
}

static void set_queen(const int column, const int row)
{
	/* Sanity check. */
	if (row < 0 || row >= max_row
		|| column < 0 || column >= max_column)
		return;

	/* Queen character, 'U' for first row and 'Q' for the rest. */
	board[row][column] = row == 0 ? 'U' : 'Q';

	/* Update free slots. */
	update_free_slots(column, row, false);
}

static void reset_queen(const int column, const int row)
{
	if (row >= 0 && row < max_row)
		if (column >= 0 && column < max_column)
			board[row][column] = '*';

	/* Update free slots. */
	update_free_slots(column, row, true);
}

static void reset_board()
{
	int column, row;

	for (row = 0; row < max_row; ++row)
		for (column = 0; column < max_column; ++column)
			reset_queen(column, row);
}

static void print_board(FILE *os)
{
	int i, j;

	xprintf(os, "\n");
	for (i = 0; i < max_row; ++i) {
		xprintf(os, "\t");
		for (j = 0; j < max_column; ++j)
			xprintf(os, "%c ", board[i][j]);
		xprintf(os, "\n");
	}
	xprintf(os, "\n");
}

static void prompt(FILE *os)
{
	xprintf(os, "Which column(0-7) do you want to put your "
		"first queen or Q for quit: ");
}

static int input(FILE *is, FILE *os)
{
	int column = max_column;
	char c, nl;
	int ret;

	ret = fscanf(is, "%c%c", &c, &nl);
	if (ret == EOF || (isalpha(c) && toupper(c) == 'Q'))
		return EOF;

	else if (isdigit(c)) {
		char str[BUFSIZ];

		str[0] = c;
		str[1] = '\0';
		column = atoi(str);
		xprintf(os, "%d\n", column);
		if (column < 0 || column >= max_column)
			column = max_column;
	}
	return column;
}

static int place_queens(const int current_row)
{
	int row = current_row;
	int column;

	if (current_row < max_row)
		for (column = 0; column < max_column; ++column) {
			if (is_free(column, current_row)) {
				set_queen(column, current_row);
				/* Recursively find the possible
				 * positions. */
				row = place_queens(current_row + 1);
				if (row != max_row) {
					/* We can't place whole queens
					 * with this position.  let's try
					 * with the next available column. */
					reset_queen(column, current_row);
					continue;
				}
			}
		}
	return row;
}

int main(int argc, char *argv[])
{
	const char *ofilename = "output.txt";
	FILE *is = stdin; /* use stdin by default. */
	FILE *fp = NULL;
	FILE *os;
	int ret;

	printf("Eight queens problem\n");
	printf("====================\n\n");

	/* Find the input file. */
	if (argc >= 2) {
		fp = fopen(argv[1], "r");
		if (fp == NULL)
			fprintf(stderr, "Can't open %s, use stdin\n",
				argv[1]);
		else
			is = fp;
	}

	/* Output file. */
	os = fopen(ofilename, "w");
	if (os == NULL) {
		printf("Can't open %s file\n", ofilename);
		exit(-1);
	}

	/* Let's find the eight queens' positions. */
	for (prompt(os); (ret = input(is, os)) != EOF; prompt(os)) {
		int row = 0;

		if (ret >= max_column) {
			xprintf(os, "\nColumn should be between 0 and %d\n\n",
				max_column - 1);
			continue;
		}
		reset_board();
		set_queen(ret, row);
		place_queens(++row);
		print_board(os);
	}

	xprintf(os, "\nThank you!\n");
	fclose(os);
	if (fp)
		fclose(fp);
}
