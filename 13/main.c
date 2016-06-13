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
		fprintf(stderr, "left child is still there\n");
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
	else {
		/* We just delete the duplicate node. */
		fprintf(stderr, "%d is duplicate. Don't add it!\n", node->value);
		free_node(node);
	}

	return root;
}

static struct node *read_data(FILE *is, FILE *os)
{
	struct node *root = NULL;
	struct node *node;
	int count = 0;
	int data;

	printf("\nRead data in the following order\n\n");

	while (fscanf(is, "%d,", &data) != EOF) {
		printf("%d, ", data);
		++count;
		node = new_node(data);
		if (node != NULL)
			root = insert(root, node);
	}
	printf("\n\nTotal number of data is %d\n", count);

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

static void print_tree_preorder(FILE *os, const struct node *const root, int *count,
		int *sum)
{
	if (root == NULL)
		return;
	xprintf(os, "%d, ", root->value);
	*sum += root->value;
	++*count;
	print_tree_preorder(os, root->left, count, sum);
	print_tree_preorder(os, root->right, count, sum);
}

static void print_tree_inorder(FILE *os, const struct node *const node, int *count,
		int *sum)
{
	if (node == NULL)
		return;
	print_tree_inorder(os, node->left, count, sum);
	xprintf(os, "%d, ", node->value);
	*sum += node->value;
	++*count;
	print_tree_inorder(os, node->right, count, sum);
}

static void print_tree_postorder(FILE *os, const struct node *const root, int *count,
		int *sum)
{
	if (root == NULL)
		return;
	print_tree_postorder(os, root->left, count, sum);
	print_tree_postorder(os, root->right, count, sum);
	xprintf(os, "%d, ", root->value);
	*sum += root->value;
	++*count;
}

struct printer {
	const char *name;
	void (*print)(FILE *os, const struct node *root, int *count, int *sum);
} printer[] = {
	{
		.name = "Preorder",
		.print = print_tree_preorder
	},
	{
		.name = "Inorder",
		.print = print_tree_inorder
	},
	{
		.name = "Postorder",
		.print = print_tree_postorder
	}
};

static void print_tree(FILE *is, FILE *os, const struct node *const tree)
{
	int count, sum;
	int i;

	for (i = count = sum = 0; printer[i].print != NULL; i++, count = sum = 0) {
		xprintf(os, "\n%d) %s traversal result\n\n", i + 1, printer[i].name);
		(*printer[i].print)(os, tree, &count, &sum);
		printf("\n\nSum is %d, out of %d number of data.", sum, count);
		xprintf(os, "\n");
	}
	xprintf(os, "\n");
}

static void handle_delete(FILE *is, FILE *os, struct node *tree)
{
	;
}

int main()
{
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;
	struct node *root = NULL;

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
	root = read_data(is, os);

	/* Process tree printing. */
	print_tree(is, os, root);

	/* Process node deletion. */
	handle_delete(is, os, root);

err:
	delete_tree(root);
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
