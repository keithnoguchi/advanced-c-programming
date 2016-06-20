/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 14

   Topic: B-tree

   file name: main.c

   Date: May 20th, 2016

   Objective: Read the following numbers from an input file in the
              same order. (Note: Do not assign an array with input
              values.)

              Using a B-tree structure read the numbers and form
              the tree.  All keys in the nodes of the left subtree
              will be lower than parent key and all keys in the right
              subtree are higher than the parent key.  Traverse the
              tree in inorder after each split.  Print the input values
              and the output node key values in inorder to output file.

   Input array: 572, 430, 315, 363, 320, 545, 451, 437, 476, 472,
                493, 395, 462, 521, 406, 412, 510, 560, 425, 595,
		580, 583, 531, 511, 459, 518, 356, 379, 488, 532

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define KEYNUM   1
#define CHILDNUM (KEYNUM + 1)

static const int invalid_index = -1;
static const int invalid_key = -999;

/* B-tree node. */
struct bnode {
	int pindex; /* parent index. */
	int keys[KEYNUM];
	struct bnode *parent;
	struct bnode *child[CHILDNUM];
};

static struct bnode *new_node(struct bnode *parent, const int pindex)
{
	struct bnode *node;
	int i;

	node = malloc(sizeof(struct bnode));
	assert(node != NULL);
	for (i = 0; i < KEYNUM; i++)
		node->keys[i] = invalid_key;
	node->pindex = pindex;
	node->parent = parent;
	for (i = 0; i < CHILDNUM; i++)
		node->child[i] = NULL;

	return node;
}

static void free_node(struct bnode *node)
{
	int i;

	assert(node->pindex == invalid_index);
	assert(node->parent == NULL);
	for (i = 0; i < KEYNUM; i++)
		assert(node->keys[i] == invalid_key);
	for (i = 0; i < CHILDNUM; i++)
		assert(node->child[i] == NULL);
	free(node);
}

static struct bnode *add_key(struct bnode *node, const int key)
{
	int i, position, total_keys;

	assert(key != invalid_key);

	if (node == NULL)
		node = new_node(NULL, invalid_index);
	assert(node != NULL);

	total_keys = 0;
	position = 0;
	for (i = 0; i < KEYNUM; i++)
		if (node->keys[i] != invalid_index) {
			if (key < node->keys[i])
				position = i;
			total_keys++;
		}
	if (total_keys < KEYNUM)
		node->keys[position] = key;
	else
		; /* not yet implemented */

	return node;
}

static struct bnode *build_tree(FILE *is, FILE *os)
{
	struct bnode *tree = NULL;
	int value;
	char comma;

	while (fscanf(is, "%d%c", &value, &comma) != EOF)
		tree = add_key(tree, 1);

	return tree;
}

/* inorder traversal. */
static void print_tree(FILE *os, const struct bnode *const tree)
{
	if (tree != NULL) {
		int i;

		print_tree(os, tree->child[0]);
		for (i = 0; i < KEYNUM; i++) {
			fprintf(os, "%d, ", tree->keys[i]);
			print_tree(os, tree->child[i + 1]);
		}
	}
}

static void delete_tree(struct bnode *tree)
{
	int i;

	/* Base case. */
	if (tree != NULL) {
		for (i = 0; i < CHILDNUM; i++) {
			delete_tree(tree->child[i]);
			tree->child[i] = NULL;
		}
		for (i = 0; i < KEYNUM; i++)
			tree->keys[i] = invalid_key;
		tree->pindex = invalid_index;
		tree->parent = NULL;
		free_node(tree);
	}
}

int main()
{
	const char *input = "input.txt", *output = "output.txt";
	FILE *is = NULL, *os = NULL;
	int ret = EXIT_SUCCESS;
	struct bnode *tree;

	is = fopen(input, "r");
	if (is == NULL) {
		fprintf(stderr, "can't open '%s' input file.\n", input);
		ret = EXIT_FAILURE;
		goto err;
	}

	os = fopen(output, "w");
	if (os == NULL) {
		fprintf(stderr, "can't open '%s' output file.\n", output);
		ret = EXIT_FAILURE;
		goto err;
	}

	/* First build tree. */
	tree = build_tree(is, os);
	if (tree == NULL) {
		fprintf(stderr, "can't build tree\n");
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Print the tree with inorder traversal. */
	print_tree(os, tree);

err:
	if (tree != NULL)
		delete_tree(tree);
	if (os != NULL)
		fclose(os);
	if (is != NULL)
		fclose(is);

	exit(ret);
}
