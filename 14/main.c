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
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#define KEYNUM   4
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

static int xprintf(FILE *os, const char *const fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	if (os != stdout) {
		va_end(ap);
		va_start(ap, fmt);
		vfprintf(os, fmt, ap);
	}
	va_end(ap);

	return ret;
}

static struct bnode *new_node(void)
{
	struct bnode *node;
	int i;

	node = malloc(sizeof(struct bnode));
	assert(node != NULL);
	for (i = 0; i < KEYNUM; i++)
		node->keys[i] = invalid_key;
	node->pindex = invalid_index;
	node->parent = NULL;
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

static bool is_node_full(const struct bnode *const node)
{
	return node->keys[KEYNUM - 1] != invalid_key;
}

static int find_position(const struct bnode *const node, const int key)
{
	int low, mid, high;

	/* There is no key in the node. */
	if (node->keys[0] == invalid_key)
		return 0;

	low = 0, high = KEYNUM - 1;
	while (low < high) {
		mid = (low + high) / 2;
		if (node->keys[mid] == invalid_key
				|| key < node->keys[mid])
			high = mid;
		else if (key > node->keys[mid])
			low = low == mid ? mid + 1 : mid;
		else
			/* No duplicate key support. */
			assert(node->keys[mid] != invalid_key);
	}
	return low;
}

static struct bnode *find_node(struct bnode *node, const int key)
{
	return node;
}

static struct bnode *split_node(struct bnode *node, const int key,
		const int position)
{
	return node; /* TBD */
}

static void insert_key(struct bnode *node, const int key, const int position)
{
	int i;

	for (i = KEYNUM - 1; i > position; i--) {
		if (node->keys[i - 1] == invalid_key)
			continue;
		node->keys[i] = node->keys[i - 1];
		node->child[i + 1] = node->child[i];
	}
	node->keys[position] = key;
	node->child[position + 1] = node->child[position];
}

static struct bnode *add_key(FILE *os, struct bnode *root, const int key,
		bool *is_split)
{
	struct bnode *node = root;
	int position;

	assert(key != invalid_key);

	if (node == NULL) {
		node = new_node();
		node->keys[0] = key;
		return node;
	}

	node = find_node(node, key);
	position = find_position(node, key);

	if (is_node_full(node)) {
		*is_split = true;
		return split_node(node, key, position);
	} else {
		if (node->keys[position] == invalid_key)
			node->keys[position] = key;
		else
			insert_key(node, key, position);
		return node;
	}
}

/* inorder traversal. */
static void print_tree(FILE *os, const struct bnode *const tree)
{
	int i;

	if (tree == NULL)
		return;

	print_tree(os, tree->child[0]);
	for (i = 0; i < KEYNUM; i++) {
		if (tree->keys[i] == invalid_key)
			break;
		xprintf(os, "%d, ", tree->keys[i]);
		print_tree(os, tree->child[i + 1]);
	}
	xprintf(os, "\n");
}

static struct bnode *build_tree(FILE *is, FILE *os)
{
	struct bnode *tree = NULL;
	bool is_split;
	char comma;
	int value;

	is_split = false;
	while (fscanf(is, "%d%c", &value, &comma) != EOF) {
		xprintf(os, "Add %d", value);
		tree = add_key(os, tree, value, &is_split);
		if (is_split)
			xprintf(os, ", triggered split: ");
		else
			xprintf(os, ": ");
		print_tree(os, tree);
		is_split = false;
	}

	return tree;
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

	xprintf(os, "\nBuilding B-tree\n");
	xprintf(os, "===============\n\n");

	/* First build tree. */
	tree = build_tree(is, os);
	if (tree == NULL) {
		fprintf(stderr, "can't build tree\n");
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Print the tree with inorder traversal. */
	xprintf(os, "\nFinal tree: ");
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
