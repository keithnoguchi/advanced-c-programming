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
#include <stdbool.h>

#define GRID_SIZE   4
static char grid[GRID_SIZE][GRID_SIZE];

static void init_grid(void)
{
	int i, j;

	for (i = 0; i < GRID_SIZE; ++i)
		for (j = 0; j < GRID_SIZE; ++j)
			grid[i][j] = '-';
}

static void print_title(void)
{
	printf("\nTic-tac-toe %dx%d version\n", GRID_SIZE, GRID_SIZE);
}

static void print_grid(void)
{
	int i, j;

	printf("\n\t  ");
	for (i = 0; i < GRID_SIZE; ++i)
		printf("%d ", i + 1);
	printf("x\n");

	for (i = 0; i < GRID_SIZE; ++i) {
		printf("\t%d ", i + 1);
		for (j = 0; j < GRID_SIZE; ++j)
			printf("%c ", grid[i][j]);
		printf("\n");
	}

	printf("\ty\n\n");
}

static void print_prompt(const char player)
{
	print_grid();
	printf("Your turn, %c. Position in x, y format or ^C to quit: ",
		player);
}

static const char get_next_player(const int count)
{
	const char player_sym[] = {'X', 'O'};
	return player_sym[count % 2];
}

static int get_position(unsigned *x, unsigned *y)
{
	int ret;

	ret = scanf("%u,%u", x, y);
	if (ret != 2)
		return EOF;

	if (*x <= 0 || *x > GRID_SIZE) {
		printf("x should be 1 <= x <= %d\n", GRID_SIZE);
		*x = 0;
	}
	if (*y <= 0 || *y > GRID_SIZE) {
		printf("y should be 1 <= y <= %d\n", GRID_SIZE);
		*y = 0;
	}

	return ret;
}

static bool is_grid_available(const int x, const int y)
{
	return grid[y - 1][x - 1] == '-';
}

static bool is_valid_position(const unsigned x, const unsigned y)
{
	if (x == 0 || y == 0 || !is_grid_available(x, y))
		return false;
	else
		return true;
}

static void fill_grid(const int x, const int y, const char player)
{
	grid[y - 1][x - 1] = player;
}

static int check_column(const int x, const int y, const char player)
{
	int count, prev_count;
	int i;

	count = prev_count = 0;
	for (i = 0; i < GRID_SIZE; ++i) {
		if (grid[i][x - 1] == player)
			++count;
		else {
			if (count > prev_count)
				prev_count = count;
			count = 0;
		}
	}
	return prev_count > count ? prev_count : count;
}

static int check_row(const int x, const int y, const char player)
{
	int count, prev_count;
	int i;

	count = prev_count = 0;
	for (i = 0; i < GRID_SIZE; ++i) {
		if (grid[y - 1][i] == player)
			++count;
		else {
			if (count > prev_count)
				prev_count = count;
			count = 0;
		}
	}
	return prev_count > count ? prev_count : count;
}

static bool is_won(const int x, const int y, const char player)
{
	const int min_win_count = GRID_SIZE - 1;
	int count;

	count = check_column(x, y, player);
	if (count >= min_win_count)
		return true;

	count = check_row(x, y, player);
	if (count >= min_win_count)
		return true;

	return false;
}

int main()
{
	unsigned x, y;
	char player;
	int count;

	init_grid();
	print_title();

	count = 0;
	player = get_next_player(count);
	for (print_prompt(player); get_position(&x, &y) != EOF;
		print_prompt(player)) {
		if (is_valid_position(x, y)) {
			fill_grid(x, y, player);
			if (is_won(x, y, player)) {
				printf("\n\tYou rock, %c!\n", player);
				break;
			} else if (count == GRID_SIZE * GRID_SIZE) {
				printf("Draw.\n");
				break;
			}
			player = get_next_player(++count);
		} else
			printf("Invalid position. Try again.\n");
	}
	print_grid();
}
