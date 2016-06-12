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
#include <stdarg.h>
#include <string.h>

struct node {
	int value;
	struct node *left;
	struct node *right;
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

static struct node *new_node(const int value)
{
	struct node *node;

	node = malloc(sizeof(struct node));
	if (node == NULL) {
		fprintf(stderr, "malloc(3)\n");
		exit(EXIT_FAILURE);
	}
	memset(node, 0, sizeof(*node));
	node->value = value;
	node->left = node->right = NULL;

	return node;
}

static struct node *free_node(struct node *node)
{
	if (node->left != NULL) {
		fprintf(stderr, "left chaild is still there\n");
		exit(EXIT_FAILURE);
	}
	if (node->right != NULL) {
		fprintf(stderr, "right child is still there\n");
		exit(EXIT_FAILURE);
	}
	free(node);
	return NULL;
}

static struct node *insert(struct node *root, struct node *node)
{
	/* Base case. */
	if (root == NULL)
		return node;
	else if (node->value < root->value)
		root->left = insert(root->left, node);
	else if (node->value > root->value)
		root->right = insert(root->right, node);
	else
		/* We just delete the duplicate node. */
		free_node(node);

	return root;
}

static struct node *create_tree(FILE *is)
{
	struct node *root = NULL;
	struct node *node;
	int data;

	while (fscanf(is, "%d,", &data) != EOF) {
		node = new_node(data);
		if (node != NULL)
			root = insert(root, node);
	}
	return root;
}

static struct node *delete_tree(struct node *root)
{
	if (root == NULL)
		return NULL;
	/* post order deletion. */
	root->left = delete_tree(root->left);
	root->right = delete_tree(root->right);
	return free_node(root);
}

static void print_tree(FILE *os, const struct node *const node)
{
	if (node == NULL)
		return;
	print_tree(os, node->left);
	xprintf(os, "%d, ", node->value);
	print_tree(os, node->right);
}

int main()
{
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;
	struct node *root;

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

	/* Create a tree based on the input file. */
	root = create_tree(is);
	print_tree(os, root);
	delete_tree(root);
	xprintf(os, "\n");

err:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
