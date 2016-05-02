/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 6

   Topic: Tic-tac-toe

   file name: main.c

   Date: May 1st, 2016

   Objective: 
*/

#include <stdio.h>

static const size_t column = 4;
static const size_t row = 4;
static char grid[4][4];

static void init_grid(void)
{
	int i, j;

	for (i = 0; i < row; ++i)
		for (j = 0; j < column; ++j)
			grid[i][j] = '-';
}

static void print_title(void)
{
	printf("\nTic-tac-toe 4x4 version\n");
}

static void print_grid(void)
{
	int i, j;

	printf("\n\t  1 2 3 4 x\n");
	for (i = 0; i < row; ++i) {
		printf("\t%d ", i + 1);
		for (j = 0; j < column; ++j)
			printf("%c ", grid[i][j]);
		printf("\n");
	}
	printf("\ty\n\n");
}

static int print_prompt(void)
{
	print_grid();
	printf("Next position in x, y or ^C to quit. > ");
	return 1;
}

static int get_input(unsigned *x, unsigned *y)
{
	int ret;

	ret = scanf("%u,%u", x, y);
	if (ret != 2)
		return EOF;

	if (*x == 0 || *x > column)
		printf("x should be 0 < x <= %d\n", column);
	if (*y == 0 || *y > row)
		printf("y should be 0 < y <= %d\n", row);

	return ret;
}

static void fill_grid(const int x, const int y)
{
	grid[y - 1][x - 1] = 'O';
}

int main()
{
	unsigned x, y;

	init_grid();
	print_title();
	for (print_prompt(); get_input(&x, &y) != EOF; print_prompt()) {
		fill_grid(x, y);
	}
}
