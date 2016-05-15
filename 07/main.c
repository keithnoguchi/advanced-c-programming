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

static const int max_row = 8;
static const int max_column = 8;

static char board[8][8] = {
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*'}
};

static void xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

static void print_board(FILE *os)
{
	int i, j;

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
	FILE *os = stdout;
	FILE *is = stdin;
	int ret;

	printf("Eight queen problem\n\n");

	for (prompt(os); (ret = input(is)) != EOF; prompt(os)) {
		if (ret >= max_column) {
			xprintf(os, "Column number should be between 0 to 7\n");
			continue;
		}
		xprintf(os, "%d\n", ret);
		print_board(NULL);
	}

	xprintf(os, "\n\nThank you!\n");
}
