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

int main()
{
	const int column = 4;
	const int row = 4;
	char board[row][column];
	int i, j;

	for (i = 0; i < row; ++i) {
		for (j = 0; j < column; ++j) {
			board[i][j] = '-';
		}
	}

	for (i = 0; i < row; ++i) {
		for (j = 0; j < column; ++j) {
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}
