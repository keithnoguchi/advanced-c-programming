/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 7

   Topic: Eight queens

   file name: main.c

   Date: May 12nd, 2016

   Objective:

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define max_row 8
#define max_column 8
static char board[max_row][max_column];

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

static void set_queen(const int column, const int row)
{
	/* Queen character, 'U' for first row and 'Q' for the rest. */
	const char queen = row == 0 ? 'U' : 'Q';

	if (row >= 0 && row < max_row)
		if (column >= 0 && column < max_column)
			board[row][column] = queen;
}

static void reset_board()
{
	int i, j;

	for (i = 0; i < max_row; ++i)
		for (j = 0; j < max_column; ++j)
			board[i][j] = '*';
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
		reset_board();
		if (ret >= max_column) {
			xprintf(os,
				"\nColumn number should be between 0 to 7\n");
			continue;
		}
		xprintf(os, "%d\n", ret);
		row = 0;
		set_queen(ret, row);
		print_board(os);
	}

	xprintf(os, "\n\nThank you!\n");
	fclose(os);
}
