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
typedef struct game {
	const size_t grid_size;
	char current_player;
	char grid[GRID_SIZE][GRID_SIZE];
} game_t;

static char grid[GRID_SIZE][GRID_SIZE];

static void init_game(game_t *game)
{
	int i, j;

	for (i = 0; i < game->grid_size; ++i)
		for (j = 0; j < game->grid_size; ++j)
			game->grid[i][j] = '-';
}

static void print_grid(const game_t *game)
{
	int i, j;

	/* Print X axis. */
	printf("\n\t  ");
	for (i = 0; i < game->grid_size; ++i)
		printf("%d ", i + 1);
	printf("x\n");

	/* Print grid as well as Y asix. */
	for (i = 0; i < game->grid_size; ++i) {
		printf("\t%d ", i + 1);
		for (j = 0; j < game->grid_size; ++j)
			printf("%c ", game->grid[i][j]);
		printf("\n");
	}
	printf("\ty\n\n");
}

static void print_prompt(const game_t *game)
{
	print_grid(game);
	printf("Player %c> ", game->current_player);
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

static bool is_grid_available(const game_t *game, const int x, const int y)
{
	return game->grid[y][x] == '-';
}

static bool is_valid_position(const game_t *game, const int x, const int y)
{
	if (x < 0 || x >= game->grid_size
		|| y < 0 || y >= game->grid_size
		|| !is_grid_available(game, x, y))
		return false;
	else
		return true;
}

static void fill_grid(game_t *game, const int x, const int y)
{
	game->grid[y][x] = game->current_player;
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

int main()
{
	game_t game = { .grid_size = GRID_SIZE };
	int x, y;
	int count;

	init_game(&game);
	printf("\nTic-tac-toe %dx%d version\n", game.grid_size, game.grid_size);
	printf("\nProvide the position in x, y format or ^C to quit.\n");

	count = 0;
	game.current_player = get_next_player(count);
	for (print_prompt(&game); get_position(&x, &y); print_prompt(&game)) {
		if (is_valid_position(&game, x, y)) {
			fill_grid(&game, x, y);
			if (is_won(&game, x, y)) {
				printf("\n\tYou rock, %c!\n",
					game.current_player);
				printf("\n\tYou won the game!\n");
				break;
			} else if (count == game.grid_size * game.grid_size) {
				printf("Draw.\n");
				break;
			}
			game.current_player = get_next_player(++count);
		} else
			printf("Invalid position. Try again.\n");
	}
	print_grid(&game);
}
