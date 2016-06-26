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

#define KEYNUM   10
#define CHILDNUM (KEYNUM + 1)

typedef char bnode_index_t;
static const bnode_index_t invalid_index = -1;
static const int invalid_key = -999;

/* B-tree node. */
struct bnode {
	bnode_index_t pindex; /* parent index. */
	bnode_index_t last;   /* last valid index. */
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
	fflush(stdout);

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
	node->pindex = node->last = invalid_index;
	node->parent = NULL;
	for (i = 0; i < CHILDNUM; i++)
		node->child[i] = NULL;

	return node;
}

static void free_node(struct bnode *node)
{
	int i;

	assert(node->pindex == invalid_index);
	assert(node->last == invalid_index);
	assert(node->parent == NULL);
	for (i = 0; i < KEYNUM; i++)
		assert(node->keys[i] == invalid_key);
	for (i = 0; i < CHILDNUM; i++)
		assert(node->child[i] == NULL);
	free(node);
}

static bool is_node_full(const struct bnode *const node)
{
	return node->last == KEYNUM - 1;
}

static bool is_node_empty(const struct bnode *const node)
{
	return node->last == invalid_key;
}

static bool is_root_node(const struct bnode *const node)
{
	return node->parent == NULL;
}

/* inorder traversal. */
static void print_tree(FILE *os, const struct bnode *const tree)
{
	int i;

	if (tree == NULL)
		return;

	xprintf(stdout, "[");
	print_tree(os, tree->child[0]);
	for (i = 0; i <= tree->last; i++) {
		xprintf(os, "%d, ", tree->keys[i]);
		print_tree(os, tree->child[i + 1]);
	}
	xprintf(stdout, "], ");
}

static struct bnode *find_position(struct bnode *node, const int key,
		bnode_index_t *position)
{
	int low, mid, high;

	while (1) {
		low = mid = 0, high = node->last;
		while (low + 1 < high) {
			mid = (low + high) / 2;
			if (key < node->keys[mid])
				high = mid;
			else if (key > node->keys[mid])
				low = mid;
			else
				/* Duplicate key is not supported. */
				assert(node->keys[mid] != invalid_key);
		}

		if (is_node_empty(node)) {
			*position = 0;
			return node;
		} else if (key < node->keys[mid]) {
			if (node->child[mid] != NULL)
				node = node->child[mid];
			else {
				*position = mid;
				return node;
			}
		} else if (key > node->keys[mid]) {
			if (node->child[mid + 1] != NULL)
				node = node->child[mid + 1];
			else {
				*position = mid + 1;
				return node;
			}
		} else {
			if (key == node->keys[mid])
				/* Duplicate key is not supported. */
				assert(node->keys[mid] != invalid_key);
			if (node->child[low + 1] != NULL)
				node = node->child[low + 1];
			else {
				*position = low + 1;
				return node;
			}
		}
	}
}

static void insert_key(struct bnode *node, const int key,
		const bnode_index_t position)
{
	int pos = position;
	int i;

	assert(node->last + 1 < KEYNUM);

	for (i = node->last; i >= pos; i--) {
		node->keys[i + 1] = node->keys[i];
		node->child[i + 1] = node->child[i];
	}
	if (pos != node->last + 1) {
		node->child[pos + 1] = node->child[pos];
		node->child[pos] = NULL;
	}
	node->keys[pos] = key;
	node->last++;
}

static struct bnode *split_node(struct bnode *node)
{
	struct bnode *parent, *right, *left = node;
	int mid = left->last / 2;
	int lindex, rindex;
	int i, j;

	if ((parent = left->parent) == NULL) {
		parent = new_node();
		lindex = 0;
	} else {
		lindex = left->pindex;
	}
	rindex = lindex + 1;

	right = new_node();
	insert_key(parent, left->keys[mid], lindex);
	parent->child[lindex] = left;
	parent->child[rindex] = right;
	left->pindex = lindex;
	right->pindex = rindex;
	left->parent = right->parent = parent;

	for (i = mid + 1, j = 0; i <= left->last; i++, j++) {
		right->keys[j] = left->keys[i];
		right->last++;
	}

	left->last = mid - 1;

	return parent;
}

static struct bnode *add_key(FILE *os, struct bnode *root, const int key,
		bool *is_split)
{
	bnode_index_t position;
	struct bnode *node;

	assert(key != invalid_key);

	if (root == NULL) {
		root = new_node();
		insert_key(root, key, 0);
		return root;
	}

	while (1) {
		node = find_position(root, key, &position);
		if (is_node_full(node)) {
			bool is_root = is_root_node(node);

			*is_split = true;
			node = split_node(node);
			if (is_root)
				root = node;
		} else {
			insert_key(node, key, position);
			break;
		}
	}
	return root;
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
		xprintf(os, "\n");
		is_split = false;
	}

	return tree;
}

static void delete_tree(struct bnode *tree)
{
	int i;

	/* Base case. */
	if (tree == NULL)
		return;

	for (i = 0; i < CHILDNUM; i++) {
		delete_tree(tree->child[i]);
		tree->child[i] = NULL;
	}
	for (i = 0; i < KEYNUM; i++)
		tree->keys[i] = invalid_key;
	tree->pindex = tree->last = invalid_index;
	tree->parent = NULL;
	free_node(tree);
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
	xprintf(os, "\n");

err:
	if (tree != NULL)
		delete_tree(tree);
	if (os != NULL)
		fclose(os);
	if (is != NULL)
		fclose(is);

	exit(ret);
}
