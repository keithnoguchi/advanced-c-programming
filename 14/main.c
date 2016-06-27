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

#define CHILDNUM  4
#define KEYNUM    (CHILDNUM - 1)

typedef char bnode_index_t;
static const bnode_index_t invalid_index = -1;
static const int invalid_key = -999;
static const bool debug = false;

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

static bool is_node_full(const struct bnode *const node)
{
	return node->last == KEYNUM - 1;
}

static bool is_node_empty(const struct bnode *const node)
{
	return node == NULL ? true : node->last == invalid_index;
}

static bool is_leaf_node(const struct bnode *const node)
{
	int i;

	for (i = 0; i < CHILDNUM; i++)
		if (node->child[i] != NULL)
			return false;

	return true;
}

static void debug_node(FILE *os, const struct bnode *const node)
{
	int i;

	if (debug == false)
		return;

	fprintf(os, "node->pindex: %d\n", node->pindex);
	fprintf(os, "node->parent: %p\n", node->parent);
	fprintf(os, "node->last: %d\n", node->last);
	for (i = 0; i <= node->last; i++)
		fprintf(os, "node->keys[%d]: %d\n", i, node->keys[i]);
}

static void print_subtree(FILE *os, const struct bnode *const tree)
{
	int i;

	if (!is_node_empty(tree->child[0])) {
		xprintf(os, "{");
		print_subtree(os, tree->child[0]);
		xprintf(os, "}, ");
	}
	for (i = 0; i < tree->last; i++) {
		xprintf(os, "%d, ", tree->keys[i]);
		if (!is_node_empty(tree->child[i + 1])) {
			xprintf(os, "{");
			print_subtree(os, tree->child[i + 1]);
			xprintf(os, "}, ");
		}
	}
	xprintf(os, "%d", tree->keys[i]);
	if (!is_node_empty(tree->child[i + 1])) {
		xprintf(os, ", {");
		print_subtree(os, tree->child[i + 1]);
		xprintf(os, "}");
	}
}

/* inorder traversal. */
static void print_tree(FILE *os, const struct bnode *const tree)
{
	int i;

	if (is_node_empty(tree))
		return;

	xprintf(os, "{");
	if (!is_node_empty(tree->child[0])) {
		xprintf(os, "{");
		print_subtree(os, tree->child[0]);
		xprintf(os, "}, ");
	}
	for (i = 0; i < tree->last; i++) {
		xprintf(os, "%d, ", tree->keys[i]);
		if (!is_node_empty(tree->child[i + 1])) {
			xprintf(os, "{");
			print_subtree(os, tree->child[i + 1]);
			xprintf(os, "}, ");
		}
	}
	xprintf(os, "%d", tree->keys[i]);
	if (!is_node_empty(tree->child[i + 1])) {
		xprintf(os, ", {");
		print_subtree(os, tree->child[i + 1]);
		xprintf(os, "}");
	}
	xprintf(os, "}\n");
}

static struct bnode *new_node(struct bnode *parent, bnode_index_t pindex)
{
	struct bnode *node;
	int pos = pindex;
	int i;

	node = malloc(sizeof(struct bnode));
	assert(node != NULL);
	for (i = 0; i < KEYNUM; i++)
		node->keys[i] = invalid_key;
	node->pindex = node->last = invalid_index;
	node->pindex = pindex;
	node->parent = parent;
	if (parent)
		parent->child[pos] = node;
	for (i = 0; i < CHILDNUM; i++)
		node->child[i] = NULL;
	debug_node(stdout, node);

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

static void insert_key(struct bnode *node, const int key,
		const bnode_index_t position)
{
	int pos = position;
	int i;

	for (i = node->last; i >= pos; i--)
		node->keys[i + 1] = node->keys[i];
	node->keys[pos] = key;
	node->last++;
}

static int find_node_position(struct bnode *node, const int key,
		int *low, int *high)
{
	int mid = *low;

	while (*low + 1 < *high) {
		mid = (*low + *high) / 2;
		if (key < node->keys[mid])
			*high = mid;
		else if (key > node->keys[mid])
			*low = mid;
		else
			/* Duplicate key is not allowed. */
			assert(node->keys[mid] != invalid_key);
	}
	return mid;
}

static struct bnode *find_leaf_node(struct bnode *root, const int key,
		bnode_index_t *position)
{
	struct bnode *node = root;
	int low, mid, high;

	while (1) {
		low = 0, high = node->last;
		mid = find_node_position(node, key, &low, &high);

		if (is_node_empty(node)) {
			*position = 0;
			return node;
		} else if (key < node->keys[low]) {
			if (node->child[low] != NULL)
				node = node->child[low];
			else {
				if (is_leaf_node(node))
					*position = low;
				else {
					*position = 0;
					node = new_node(node, low);
				}
				return node;
			}
		} else if (key > node->keys[high]) {
			if (node->child[high + 1] != NULL)
				node = node->child[high + 1];
			else {
				if (is_leaf_node(node))
					*position = high + 1;
				else {
					*position = 0;
					node = new_node(node, high + 1);
				}
				return node;
			}
		} else {
			if (key > node->keys[mid]) {
				if (node->child[mid + 1] != NULL)
					node = node->child[mid + 1];
				else {
					if (is_leaf_node(node))
						*position = mid + 1;
					else {
						*position = 0;
						node = new_node(node, mid + 1);
					}
					return node;
				}
			} else if (key < node->keys[mid]) {
				if (node->child[mid] != NULL)
					node = node->child[mid];
				else {
					if (is_leaf_node(node))
						*position = mid;
					else {
						*position = 0;
						node = new_node(node, mid);
					}
					return node;
				}
			} else {
				if (key == node->keys[mid])
					/* Duplicate key is not allowed. */
					assert(node->keys[mid] != invalid_key);
			}
		}
	}
}

static void insert_key_to_parent(struct bnode *node, const int key_index,
		struct bnode *parent, const int pindex)
{
	int i;

	assert(!is_node_full(parent));

	for (i = parent->last; i >= pindex; i--) {
		parent->child[i + 2] = parent->child[i + 1];
		if (parent->child[i + 2])
			parent->child[i + 2]->pindex++;
		parent->keys[i + 1] = parent->keys[i];
	}
	if (pindex != parent->last + 1) {
		parent->child[pindex + 1] = parent->child[pindex];
		if (parent->child[pindex + 1])
			parent->child[pindex + 1]->pindex++;
		parent->child[pindex] = NULL;
	}
	parent->keys[pindex] = node->keys[key_index];
	node->keys[key_index] = invalid_key;
	parent->child[pindex] = node;
	parent->last++;
}

static struct bnode *move_keys_to_sibling(struct bnode *node,
		const int key_index, struct bnode *parent, const int pindex)
{
	struct bnode *sibling;
	int i, j;

	sibling = new_node(parent, pindex + 1);
	for (i = key_index + 1, j = 0; i <= node->last; i++, j++) {
		insert_key(sibling, node->keys[i], j);
		node->keys[i] = invalid_key;
		sibling->child[j] = node->child[i];
		if (sibling->child[j]) {
			sibling->child[j]->parent = sibling;
			sibling->child[j]->pindex = j;
			node->child[i] = NULL;
		}
	}
	sibling->child[j] = node->child[i];
	if (sibling->child[j]) {
		sibling->child[j]->parent = sibling;
		sibling->child[j]->pindex = j;
		node->child[i] = NULL;
	}

	node->last = key_index - 1;

	return sibling;
}

static struct bnode *split_parent(struct bnode *node, const int key_index,
		struct bnode **root)
{
	struct bnode *parent, *sibling;
	int mid = node->last / 2;
	int pindex;

	if ((parent = node->parent) == NULL) {
		parent = new_node(NULL, invalid_index);
		*root = node->parent = parent;
		pindex = node->pindex = 0;
	} else {
		if (is_node_full(parent))
			parent = split_parent(parent, node->pindex, root);
		pindex = node->pindex;
	}

	insert_key_to_parent(node, mid, parent, pindex);
	sibling = move_keys_to_sibling(node, mid, parent, pindex);

	return key_index <= mid ? node : sibling;
}

static struct bnode *split_leaf_node(struct bnode *node, const int key,
		bnode_index_t *position, struct bnode **root)
{
	struct bnode *parent, *sibling;
	int mid = node->last / 2;
	int pindex;

	if ((parent = node->parent) == NULL) {
		parent = new_node(NULL, invalid_index);
		*root = node->parent = parent;
		pindex = node->pindex = 0;
	} else {
		if (is_node_full(parent))
			parent = split_parent(parent, node->pindex, root);
		pindex = node->pindex;
	}

	insert_key_to_parent(node, mid, parent, pindex);
	sibling = move_keys_to_sibling(node, mid, parent, pindex);

	if (*position <= mid)
		return find_leaf_node(node, key, position);
	else
		return find_leaf_node(sibling, key, position);
}

static bool add_key(FILE *os, struct bnode **root, const int key)
{
	bnode_index_t position;
	struct bnode *node;
	bool is_split = false;

	assert(key != invalid_key);

	if (*root == NULL) {
		*root = new_node(NULL, 0);
		insert_key(*root, key, 0);
		return is_split;
	}

	node = find_leaf_node(*root, key, &position);
	if (is_node_full(node)) {
		node = split_leaf_node(node, key, &position, root);
		is_split = true;
	}
	insert_key(node, key, position);

	return is_split;
}

static struct bnode *build_tree(FILE *is, FILE *os)
{
	struct bnode *tree = NULL;
	bool is_split;
	char comma;
	int value;
	int i = 0;

	xprintf(os, "%d) ", ++i);
	while (fscanf(is, "%d%c", &value, &comma) != EOF) {
		xprintf(os, "%d", value);
		is_split = add_key(os, &tree, value);
		if (is_split) {
			xprintf(os, " triggers split ");
			xprintf(os, "and makes a tree as below:\n\n");
			print_tree(os, tree);
			xprintf(os, "\n%d) ", ++i);
		} else {
			xprintf(os, ", ");
			if (debug)
				print_tree(stdout, tree);
		}
	}

	return tree;
}

static void delete_tree(struct bnode *tree)
{
	int i;

	/* Base case. */
	if (tree == NULL)
		return;

	for (i = 0; i <= tree->last + 1; i++) {
		if (i <= tree->last)
			tree->keys[i] = invalid_key;
		delete_tree(tree->child[i]);
		tree->child[i] = NULL;
	}
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

	xprintf(os, "\nBuilding %d-way B-tree\n", CHILDNUM);
	xprintf(os, "=====================\n\n");

	/* Let's build tree. */
	tree = build_tree(is, os);
	if (tree == NULL) {
		fprintf(stderr, "can't build tree\n");
		ret = EXIT_FAILURE;
		goto err;
	}

	/* Print the tree with inorder traversal. */
	xprintf(os, "Final B-tree\n\n");
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
