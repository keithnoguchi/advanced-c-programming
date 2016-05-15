/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 7

   Topic: Eight queens

   file name: main.c

   Date: May 14th, 2016

   Objective: Place 8 queens on an 8 x 8 chess board so that none of them
              the queens attack each other.
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

static bool set_queen(const int column, const int row)
{
	/* Sanity check. */
	if (row < 0 || row >= max_row
		|| column < 0 || column >= max_column)
		return false;

	if (!is_free(column, row))
		return false;

	/* Queen character, 'U' for first row and 'Q' for the rest. */
	board[row][column] = row == 0 ? 'U' : 'Q';

	/* Unset the free slots. */
	free_column[column] = false;
	free_up[column + row] = false;
	free_down[max_column - 1 - column + row] = false;

	return true;
}

static void reset_queen(const int column, const int row)
{
	if (row >= 0 && row < max_row)
		if (column >= 0 && column < max_column)
			board[row][column] = '*';

	/* Set the free slots. */
	free_column[column] = true;
	free_up[column + row] = true;
	free_down[max_column - 1 - column + row] = true;
}

static void reset_board()
{
	int i, j;

	/* Reset the boards. */
	for (i = 0; i < max_row; ++i)
		for (j = 0; j < max_column; ++j)
			reset_queen(j, i);

	/* Reset the free slots. */
	for (i = 0; i < max_column; ++i)
		free_column[i] = true;

	/* Reset the up and down diagonal slots. */
	for (i = 0; i < max_column + max_row - 1; ++i) {
		free_up[i] = true;
		free_down[i] = true;
	}
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

static int input(FILE *is)
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
		if (column < 0 || column >= max_column)
			column = max_column;
	}
	return column;
}

static int find_queens(const int current_row)
{
	;
}

int main()
{
	const char *ofilename = "output.txt";
	FILE *is = stdin;
	FILE *os = NULL;
	int row;
	int ret;

	printf("Eight queen problem\n\n");

	os = fopen(ofilename, "w");
	if (os == NULL) {
		printf("Can't open %s file\n", ofilename);
		exit(-1);
	}

	for (prompt(os); (ret = input(is)) != EOF; prompt(os)) {
		if (ret >= max_column) {
			xprintf(os,
				"\nColumn number should be between 0 to 7\n");
			continue;
		}
		xprintf(os, "%d\n", ret);
		row = 0;
		reset_board();
		set_queen(ret, row);
		find_queens(++row);
		print_board(os);
	}

	xprintf(os, "\n\nThank you!\n");
	fclose(os);
}
