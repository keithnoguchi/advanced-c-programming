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

              1) Traverse the tree in preorder, inorder, postorder, and
              print the data value (info) of the node when the node is
              visited.

	      2) Write delete algorithm to delete nodes and replacing them
              with their inorder successor.  Print value inorder after
              all deletes.

              Write both non-recursive and recursive routines to traverse
              the trees.

   Input array:  55, 62, 89, 85, 97, 56, 71, 82, 38, 49, 25, 67, 58, 92,
                100, 44, 69, 72, 65, 52, 41, 84, 21, 60, 95, 12,
                 35, 42, 105, 99, 34, 47, 35, 79, 95, 50, 25, 51
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* List for stack implementation. */
struct list {
	void *data;
	struct list *next;
};

/* Maintain the list of node, easy traversal. */
struct list *tree_head = NULL;

struct node {
	int value;
	bool processed; /* For iterative process. */
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

struct list *list_new(void *data)
{
	struct list *list;

	list = malloc(sizeof(struct list));
	if (list == NULL)
		return NULL;

	list->next = NULL;
	list->data = data;

	return list;
}

static void list_free(struct list *list)
{
	assert(list->next == NULL);
	free(list);
}

static void list_iterate(struct list *head, void (*func)(void *data))
{
	struct list *list;

	for (list = head; list != NULL; list = list->next)
		(*func)(list->data);
}

static struct list *list_delete(struct list *head, const void *data)
{
	struct list *list, *prev;

	for (list = prev = head; list != NULL; list = list->next) {
		if (list->data == data) {
			if (list == head)
				head = list->next;
			else
				prev->next = list->next;
			list->next = NULL;
			list_free(list);
			break;
		}
		prev = list;
	}
	return head;
}

static void list_delete_all(struct list *head)
{
	struct list *next;

	while (head) {
		next = head->next;
		free(head);
		head = next;
	}
}

static struct list *push(struct list *head, struct node *node)
{
	struct list *list;

	list = list_new(node);
	if (list != NULL)
		list->next = head;

	return list;
}

static void *pop(struct list **head)
{
	struct list *list;
	void *data = NULL;

	list = *head;
	while (list != NULL) {
		*head = list->next;
		data = list->data;
		list->data = NULL;
		list->next = NULL;
		list_free(list);
		if (data != NULL)
			return data;
		list = *head;
	}
	return NULL;
}

static struct node *new_node(const int value)
{
	struct node *node;

	node = malloc(sizeof(struct node));
	assert(node);
	memset(node, 0, sizeof(*node));
	node->value = value;
	node->left = node->right = NULL;
	node->processed = false;
	tree_head = push(tree_head, node);

	return node;
}

static struct node *free_node(struct node *node)
{
	assert(node->processed == false);
	assert(node->left == NULL);
	assert(node->right == NULL);
	tree_head = list_delete(tree_head, node);
	free(node);
	return NULL;
}

static void reset_node_flag(void *data)
{
	struct node *node = data;
	node->processed = false;
}

static void reset_tree(void)
{
	list_iterate(tree_head, reset_node_flag);
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
		fprintf(stderr, "%d is duplicate. Don't add it!\n",
				node->value);
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

static void print_node(FILE *os, struct node *const node, int *count, int *sum)
{
	xprintf(os, "%d, ", node->value);
	node->processed = true;
	*sum += node->value;
	++*count;
}

static void print_tree_preorder(FILE *os, struct node *const root, int *count,
		int *sum)
{
	if (root == NULL)
		return;
	print_node(os, root, count, sum);
	print_tree_preorder(os, root->left, count, sum);
	print_tree_preorder(os, root->right, count, sum);
}

static void print_tree_inorder(FILE *os, struct node *const node, int *count,
		int *sum)
{
	if (node == NULL)
		return;
	print_tree_inorder(os, node->left, count, sum);
	print_node(os, node, count, sum);
	print_tree_inorder(os, node->right, count, sum);
}

static void print_tree_postorder(FILE *os, struct node *const root, int *count,
		int *sum)
{
	if (root == NULL)
		return;
	print_tree_postorder(os, root->left, count, sum);
	print_tree_postorder(os, root->right, count, sum);
	print_node(os, root, count, sum);
}

static void print_tree_inorder_iterative(FILE *os, struct node *const root,
		int *count, int *sum)
{
	struct node *node = (struct node *) root;
	struct list *stack = NULL;
	struct list *list;

	while (node) {
		if (node->left && node->left->processed == false) {
			stack = push(stack, node);
			node = node->left;
		} else if (node->processed == false) {
			print_node(os, node, count, sum);
			if (node->right) {
				stack = push(stack, node);
				node = node->right;
			} else {
				while ((node = pop(&stack)))
					if (node->processed == false)
						break;
			}
		} else
			break;
	}

	list_delete_all(stack);
}

struct printer {
	const char *name;
	void (*print_recursive)(FILE *os, struct node *const root, int *count,
			int *sum);
	void (*print_iterative)(FILE *os, struct node *const root, int *count,
			int *sum);
} printer[] = {
	{
		.name = "Preorder",
		.print_recursive = print_tree_preorder,
		.print_iterative = NULL
	},
	{
		.name = "Inorder",
		.print_recursive = print_tree_inorder,
		.print_iterative = print_tree_inorder_iterative
	},
	{
		.name = "Postorder",
		.print_recursive = print_tree_postorder,
		.print_iterative = NULL
	},
	{
		.name = NULL
	}
};

static void print_tree(FILE *is, FILE *os, struct node *const tree)
{
	int count, sum;
	int i;

	xprintf(os, "\nTree traversal\n");
	xprintf(os, "--------------\n");

	for (i = count = sum = 0; printer[i].name != NULL; i++, count = sum = 0) {
		if (printer[i].print_recursive) {
			xprintf(os,
				"\n%d.1) %s recursive traversal\n\n",
				i + 1, printer[i].name);
			(*printer[i].print_recursive)(os, tree, &count, &sum);
			printf("\n\nSum is %d, out of %d number of data.",
				sum, count);
			xprintf(os, "\n");
			reset_tree();
		}
		if (printer[i].print_iterative) {
			count = sum = 0;
			xprintf(os,
				"\n%d.2) %s iterative traversal\n\n",
				i + 1, printer[i].name);
			(*printer[i].print_iterative)(os, tree, &count, &sum);
			printf("\n\nSum is %d, out of %d number of data.",
				sum, count);
			xprintf(os, "\n");
			reset_tree();
		}
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

	xprintf(os, "Binary tree traversal and removal\n");
	xprintf(os, "=================================\n");

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
