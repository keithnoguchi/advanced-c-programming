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
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#define GRID_SIZE   4
typedef struct game {
	const size_t grid_size;
	size_t filled_grid;
	char current_player;
	char grid[GRID_SIZE][GRID_SIZE];
} game_t;

static void init_game(game_t *game)
{
	int i, j;

	game->filled_grid = 0;
	for (i = 0; i < game->grid_size; ++i)
		for (j = 0; j < game->grid_size; ++j)
			game->grid[i][j] = '-';
}

static int xprintf(FILE *fp, const char *fmt, ...)
{
	va_list ap;
	int ret;

	if (fp != stdout) {
		/* Print out to the file. */
		va_start(ap, fmt);
		vfprintf(fp, fmt, ap);
		va_end(ap);
	}
	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	va_end(ap);

	return ret;
}

static void print_grid(FILE *fp, const game_t *game)
{
	int i, j;

	/* Print X axis. */
	xprintf(fp, "\n\t  ");
	for (i = 0; i < game->grid_size; ++i)
		xprintf(fp, "%d ", i + 1);
	xprintf(fp, "x\n");

	/* Print grid as well as Y asix. */
	for (i = 0; i < game->grid_size; ++i) {
		xprintf(fp, "\t%d ", i + 1);
		for (j = 0; j < game->grid_size; ++j)
			xprintf(fp, "%c ", game->grid[i][j]);
		xprintf(fp, "\n");
	}
	xprintf(fp, "\ty\n\n");
}

static void print_prompt(FILE *fp, const game_t *game)
{
	print_grid(fp, game);
	xprintf(fp, "Player %c> ", game->current_player);
}

static const char get_next_player(const game_t *game)
{
	const char player_sym[] = {'X', 'O'};
	return player_sym[game->filled_grid % 2];
}

static int get_position(FILE *fp, int *x, int *y)
{
	int ret;

	ret = scanf("%d,%d", x, y);
	if (ret != 2)
		return 0;

	/* Echo back to the output file. */
	fprintf(fp, "%d,%d\n", *x, *y);

	/* Adjust the index to the internal representation. */
	*x -= 1;
	*y -= 1;

	return 1;
}

static bool is_grid_available(const game_t *game, const int x, const int y)
{
	return game->grid[y][x] == '-';
}

static bool is_grid_filled(const game_t *game)
{
	return game->filled_grid == game->grid_size * game->grid_size;
}

static bool is_valid_position(const game_t *game, const int x, const int y)
{
	if (x < 0 || x >= game->grid_size || y < 0 || y >= game->grid_size
		|| !is_grid_available(game, x, y))
		return false;
	else
		return true;
}

static void fill_grid(game_t *game, const int x, const int y)
{
	game->grid[y][x] = game->current_player;
	game->filled_grid += 1;
}

static int check_column(const game_t *game, const int x, const int y)
{
	int count = 1;
	int i;

	/* Check the top side. */
	for (i = y - 1; i >= 0 && game->grid[i][x] == game->current_player; --i)
		++count;

	/* Check the bottom side. */
	for (i = y + 1;
		i < game->grid_size
		&& game->grid[i][x] == game->current_player;
		++i)
		++count;

	return count;
}

static int check_row(const game_t *game, const int x, const int y)
{
	int count = 1;
	int i;

	/* Check the left side. */
	for (i = x - 1; i >= 0 && game->grid[y][i] == game->current_player; --i)
		++count;

	/* Check thr right side. */
	for (i = x + 1;
		i < game->grid_size
		&& game->grid[y][i] == game->current_player;
		++i)
		++count;

	return count;
}

static int check_diagonal_1(const game_t *game, const int x, const int y)
{
	int count = 1;
	int i, j;

	/* Check the left-top side. */
	for (i = x - 1, j = y - 1;
		i >= 0 && j >= 0 && game->grid[j][i] == game->current_player;
		--i, --j)
		++count;

	/* Check the right-bottom side. */
	for (i = x + 1, j = y + 1;
		i < game->grid_size && j < game->grid_size
		&& game->grid[j][i] == game->current_player;
		++i, ++j)
		++count;

	return count;
}

static int check_diagonal_2(const game_t *game, const int x, const int y)
{
	int count = 1;
	int i, j;

	/* Check the left-bottom side. */
	for (i = x - 1, j = y + 1;
		i >= 0 && j < game->grid_size
		&& game->grid[j][i] == game->current_player;
		--i, ++j)
		++count;

	/* Check the right-top side. */
	for (i = x + 1, j = y - 1;
		i < game->grid_size && j >= 0
		&& game->grid[j][i] == game->current_player;
		++i, --j)
		++count;

	return count;
}

static bool is_won(const game_t *game, const int x, const int y)
{
	const int min_win_count = game->grid_size - 1;
	int count;

	count = check_column(game, x, y);
	if (count >= min_win_count)
		return true;

	count = check_row(game, x, y);
	if (count >= min_win_count)
		return true;

	count = check_diagonal_1(game, x, y);
	if (count >= min_win_count)
		return true;

	count = check_diagonal_2(game, x, y);
	if (count >= min_win_count)
		return true;

	return false;
}

int main(int argc, char *argv[])
{
	game_t game = { .grid_size = GRID_SIZE };
	char *output_file = "output.txt";
	FILE *fp;
	int x, y;

	if (argc >= 2)
		output_file = argv[1];

	fp = fopen(output_file, "w");
	if (fp == NULL) {
		fprintf(stderr, "Can't open %s\n", output_file);
		exit(-1);
	}

	init_game(&game);
	xprintf(fp, "\nTic-tac-toe %dx%d version\n",
			game.grid_size, game.grid_size);
	xprintf(fp, "\nProvide the position in x, y format or ^C to exit.\n");

	game.current_player = get_next_player(&game);
	for (print_prompt(fp, &game); get_position(fp, &x, &y);
		print_prompt(fp, &game)) {
		if (is_valid_position(&game, x, y)) {
			fill_grid(&game, x, y);
			if (is_won(&game, x, y)) {
				xprintf(fp, "\n\tYou rock, %c!\n",
					game.current_player);
				xprintf(fp, "\n\tYou won the game!\n");
				break;
			} else if (is_grid_filled(&game)) {
				xprintf(fp, "\n\tIt's draw.\n");
				break;
			}
			game.current_player = get_next_player(&game);
		} else
			xprintf(fp, "Invalid position. Try again.\n");
	}
	print_grid(fp, &game);
	fclose(fp);
}
