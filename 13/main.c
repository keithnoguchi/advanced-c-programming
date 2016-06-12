/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 13

   Topic: binary tree traversal

   file name: main.c

   Date: May 12th, 2016

   Objective: Read the following numbers from an input file directory
              as integer values in to a binary tree.  Using a binary
              tree structure read the number and form an inordered tree.
              All nodes to left subtree of the root will be lower than
              root and all nodes in the right subtree are higher.
              Traverse the tree in preorder, inorder, postorder, and
              print the data value (info) of the node when the node is
              visited.  Write delete algorithm to delete nodes and
              replacing them with their inorder successor.  Print value
              inorder after all deletes.  Write both non-recursive and
              recursive routines to traverse the trees.

   Input array:  55, 62, 89, 85, 97, 56, 71, 82, 38, 49, 25, 67, 58, 92,
                100, 44, 69, 72, 65, 52, 41, 84, 21, 60, 95, 12,
                 35, 42, 105, 99, 34, 47, 35, 79, 95, 50, 25, 51
*/

#include <stdio.h>
#include <stdlib.h>

static void read_and_write_data(FILE *is, FILE *os)
{
	int data;
	int ret;

	while (fscanf(is, "%d,", &data) != EOF) {
		fprintf(os, "%d, ", data);
	}
	fprintf(os, "\n");
}

int main()
{
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;

	is = fopen(input, "r");
	if (is == NULL) {
		fprintf(stderr, "Can't optn %s for read\n", input);
		goto err;
	}

	os = fopen(output, "w");
	if (os == NULL) {
		fprintf(stderr, "Can't open %s for output\n", output);
		goto err;
	}

	read_and_write_data(is, os);

err:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
