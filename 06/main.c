/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 6

   Topic: Tic-tac-toe

   file name: main.c

   Date: May 1st, 2016

   Objective: Implement the game tic-tac-toe as described below.
              Show the input and output of the game and finally the result.
              You may use a 4 X 4 array and you are free to use the
              algorithm described in the sourse material.  Show the
              initial board, the moves of each player, and the result
              of the game including the final board.

*/

#include <stdio.h>
#include <stdbool.h>

#define GRID_SIZE   4
static char grid[GRID_SIZE][GRID_SIZE];

static void init_grid(const size_t grid_size)
{
	int i, j;

	for (i = 0; i < grid_size; ++i)
		for (j = 0; j < grid_size; ++j)
			grid[i][j] = '-';
}

static void print_grid(const size_t grid_size)
{
	int i, j;

	/* Print X axis. */
	printf("\n\t  ");
	for (i = 0; i < grid_size; ++i)
		printf("%d ", i + 1);
	printf("x\n");

	/* Print grid as well as Y asix. */
	for (i = 0; i < grid_size; ++i) {
		printf("\t%d ", i + 1);
		for (j = 0; j < grid_size; ++j)
			printf("%c ", grid[i][j]);
		printf("\n");
	}
	printf("\ty\n\n");
}

static void print_prompt(const char player, const size_t grid_size)
{
	print_grid(grid_size);
	printf("Player %c> ", player);
}

static const char get_next_player(const int count)
{
	const char player_sym[] = {'X', 'O'};
	return player_sym[count % 2];
}

static int get_position(int *x, int *y)
{
	int ret;

	ret = scanf("%d,%d", x, y);
	if (ret != 2)
		return 0;

	/* Adjust the index to the internal representation. */
	*x -= 1;
	*y -= 1;

	return 1;
}

static bool is_grid_available(const int x, const int y)
{
	return grid[y][x] == '-';
}

static bool is_valid_position(const int x, const int y, const size_t grid_size)
{
	if (x < 0 || x >= grid_size || y < 0 || y >= grid_size
			|| !is_grid_available(x, y))
		return false;
	else
		return true;
}

static void fill_grid(const int x, const int y, const char player)
{
	grid[y][x] = player;
}

static int check_column(const int x, const int y, const char player,
		const size_t grid_size)
{
	int count = 1;
	int i;

	/* Check the top side. */
	for (i = y - 1; i >= 0 && grid[i][x] == player; --i)
		++count;

	/* Check the bottom side. */
	for (i = y + 1; i < grid_size && grid[i][x] == player; ++i)
		++count;

	return count;
}

static int check_row(const int x, const int y, const char player,
		const size_t grid_size)
{
	int count = 1;
	int i;

	/* Check the left side. */
	for (i = x - 1; i >= 0 && grid[y][i] == player; --i)
		++count;

	/* Check thr right side. */
	for (i = x + 1; i < grid_size && grid[y][i] == player; ++i)
		++count;

	return count;
}

static int check_diagonal_1(const int x, const int y, const char player,
		const size_t grid_size)
{
	int count = 1;
	int i, j;

	/* Check the left-top side. */
	for (i = x - 1, j = y - 1;
		i >= 0 && j >= 0 && grid[j][i] == player;
		--i, --j)
		++count;

	/* Check the right-bottom side. */
	for (i = x + 1, j = y + 1;
		i < grid_size && j < grid_size && grid[j][i] == player;
		++i, ++j)
		++count;

	return count;
}

static int check_diagonal_2(const int x, const int y, const char player,
		const size_t grid_size)
{
	int count = 1;
	int i, j;

	/* Check the left-bottom side. */
	for (i = x - 1, j = y + 1;
		i >= 0 && j < grid_size && grid[j][i] == player;
		--i, ++j)
		++count;

	/* Check the right-top side. */
	for (i = x + 1, j = y - 1;
		i < grid_size && j >= 0 && grid[j][i] == player;
		++i, --j)
		++count;

	return count;
}

static bool is_won(const int x, const int y, const char player,
		const size_t grid_size)
{
	const int min_win_count = grid_size - 1;
	int count;

	count = check_column(x, y, player, grid_size);
	if (count >= min_win_count)
		return true;

	count = check_row(x, y, player, grid_size);
	if (count >= min_win_count)
		return true;

	count = check_diagonal_1(x, y, player, grid_size);
	if (count >= min_win_count)
		return true;

	count = check_diagonal_2(x, y, player, grid_size);
	if (count >= min_win_count)
		return true;

	return false;
}

int main()
{
	int x, y;
	char player;
	int count;

	init_grid(GRID_SIZE);
	printf("\nTic-tac-toe %dx%d version\n", GRID_SIZE, GRID_SIZE);
	printf("\nProvide the position in x, y format or ^C to quit.\n");

	count = 0;
	player = get_next_player(count);
	for (print_prompt(player, GRID_SIZE);
		get_position(&x, &y);
		print_prompt(player, GRID_SIZE)) {
		if (is_valid_position(x, y, GRID_SIZE)) {
			fill_grid(x, y, player);
			if (is_won(x, y, player, GRID_SIZE)) {
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
	print_grid(GRID_SIZE);
}
