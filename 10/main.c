/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 10

   Topic: Tower of Hanoi, the power of recursion

   file name: main.c

   Date: June 30th, 2016

   Background: The problem of towers of Hanoi has five, it's
               arbitrary though, disks of differing graded diameters
               are placed on peg A, so that a larger disks is always
               below a smaller disk. The objective is to move the five
               disks to peg C, using peg B as auxiliary.  Only the top
               disk on any peg may be moved to any other peg, and
               a larger disk may never rest on a smaller one.

   Objective: Prompt the user to give the number of disks
              to be placed on the target post(C) from the initial
              post(A), using the auxilary post(B).  Three outputs
              are expected for this assignment, with 4, 5, and 6
              disks by prompting the user to specify the number
              of disks within the range.

	      Also, print each move of every disk from any peg
              to any peg.

    Hint: The solution needs two recursive calls in the main program.

          One is to place n-1 disks recursively from the from pole(A)
          to the auxiliary pole(B) and place disk n from the from pole(A)
          to the to pole(C).

          Second one is to place n-1 disks recursively from the auxiliary
          pole(B) to the to pole(C) using the from pole(A) as auxiliary
          pole.

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* Supported parameters. */
static struct parameter {
	const int height;
	const char *const output;
} supported_params[] = {
#undef DEBUG
#ifdef DEBUG
	{
		.height = 1,
		.output = "output1.txt"
	},
	{
		.height = 2,
		.output = "output2.txt"
	},
	{
		.height = 3,
		.output = "output3.txt"
	},
#endif /* DEBUG */
	{
		.height = 4,
		.output = "output4.txt"
	},
	{
		.height = 5,
		.output = "output5.txt"
	},
	{
		.height = 6,
		.output = "output6.txt"
	},
	{
		.height = -1,
		.output = NULL
	}
};

/* Disk, which points the next disk below. */
struct disk {
	unsigned char number;
	struct disk *next;
};

/* Tower, which has the current height of the disks
 * with the pointer to the top of the disk. */
struct tower {
	int height;
	struct disk *top;
};

/* Tower of Hanoi top level instance. */
struct towers {
	int max_height;
	struct tower *from;
	struct tower *aux;
	struct tower *to;
	FILE *os; /* Output stream pointer. */
};

static int xprintf(FILE *os, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vprintf(fmt, ap);
	if (os != stdout && os != stderr) {
		va_end(ap);
		va_start(ap, fmt);
		vfprintf(os, fmt, ap);
	}
	va_end(ap);

	return ret;
}

static struct disk *new_disk(const unsigned char number)
{
	struct disk *disk;

	disk = malloc(sizeof(struct disk));
	assert(disk != NULL);
	disk->number = number;
	disk->next = NULL;

	return disk;
}

static void free_disk(struct disk *disk)
{
	assert(disk->next == NULL);
	free(disk);
}

static struct disk *pop(struct disk **top)
{
	if (*top != NULL) {
		struct disk *disk = *top;
		*top = disk->next;
		disk->next = NULL;
		return disk;
	} else
		return NULL;
}

static struct disk *push(struct disk *top, struct disk *const new_disk)
{
	new_disk->next = top;
	return new_disk;
}

static int print_disk(FILE *os, const struct disk *const disk)
{
	return xprintf(os, "%u", disk->number);
}

static struct tower *new_tower(const int height)
{
	struct tower *t;
	int i;

	t = malloc(sizeof(struct tower));
	assert(t != NULL);
	t->top = NULL;
	t->height = height;

	for (i = t->height; i > 0; i--)
		t->top = push(t->top, new_disk(i));

	return t;
}

static void delete_tower(struct tower *t)
{
	struct disk *disk;
	/* Let's drain the disks. */
	while ((disk = pop(&t->top)))
		free_disk(disk);
	t->height = 0;
	free(t);
}

static struct disk *pop_disk(struct tower *t)
{
	if (t->height == 0)
		return NULL;

	t->height--;
	return pop(&t->top);
}

static void push_disk(struct tower *t, struct disk *const disk)
{
	t->top = push(t->top, disk);
	t->height++;
}

static int print_tower(const struct towers *const top,
		const struct tower *const t, const int height)
{
	FILE *os = top->os;
	int ret = 0;

	if (t == NULL || t->height < height)
		ret += xprintf(os, " ");
	else {
		int diff = t->height - height;
		struct disk *disk;
		int i;

		/* Find the target disk. */
		disk = t->top;
		for (i = 0; i < t->height; i++) {
			if (i == diff) {
				ret += print_disk(os, disk);
				break;
			}
			disk = disk->next;
		}
	}

	return ret;
}

static void init_towers(struct towers *top, const struct parameter *const param)
{
	FILE *os;

	top->max_height = param->height;
	top->os = fopen(param->output, "w");
	assert(os != NULL);
	top->from = new_tower(top->max_height);
	assert(top->from != NULL);
	top->aux = new_tower(0);
	assert(top->aux != NULL);
	top->to = new_tower(0);
	assert(top->to != NULL);
}

static void term_towers(struct towers *top)
{
	if (top->from) {
		delete_tower(top->from);
		top->from = NULL;
	}
	if (top->aux) {
		delete_tower(top->aux);
		top->aux = NULL;
	}
	if (top->to) {
		delete_tower(top->to);
		top->to = NULL;
	}
	if (top->os != NULL) {
		fclose(top->os);
		top->os = NULL;
	}
}

static void print_header(const struct towers *const top)
{
	xprintf(top->os, "\nCurrent towers\n");
	xprintf(top->os, "--------------\n");
}

static void print_footer(const struct towers *const top)
{
	xprintf(top->os, "--------------\n");
	xprintf(top->os, "A     B     C\n\n");
}

static void print_towers(const struct towers *const top)
{
	FILE *os = top->os;
	int i;

	print_header(top);

	for (i = top->max_height; i > 0; i--) {
		print_tower(top, top->from, i);
		xprintf(os, "     ");
		print_tower(top, top->aux, i);
		xprintf(os, "     ");
		print_tower(top, top->to, i);
		xprintf(top->os, "\n");
	}
	print_footer(top);
}

static void prompt(FILE *os)
{
	int i;

	fprintf(os, "Tell me the height of the initial tower, e.g. ");
	for (i = 0; supported_params[i].output != NULL; i++) {
		xprintf(os, "%d, ", supported_params[i].height);
	}
	fprintf(os, "or -1 to quit? ");
}

static struct parameter *const input(FILE *is, FILE *os)
{
	int number;
	char c;
	int i;
	int ret;

	ret = fscanf(is, "%d%c", &number, &c);
	xprintf(os, "%d\n", number);

	if (ret != 2 || number == -1)
		return NULL;

	for (i = 0; supported_params[i].output != NULL; i++)
		if (number == supported_params[i].height)
			return &supported_params[i];

	return &supported_params[i];
}

static void move_disks(struct towers *top, struct tower *from,
		struct tower *to, struct tower *aux, int from_height)
{
	/* Base case. */
	if (from_height == 0)
		return;

	/* second case. */
	else if (from_height == 1) {
		push_disk(to, pop_disk(from));
		print_towers(top);
		return;
	}

	move_disks(top, from, aux, to, from_height - 1);
	move_disks(top, from, to, aux, 1);
	move_disks(top, aux, to, from, from_height - 1);
}

static void run(struct towers *top, const struct parameter *const param)
{
	/* Initialize towers. */
	init_towers(top, param);

	/* Print the initial state of the towers. */
	print_towers(top);

	move_disks(top, top->from, top->aux, top->to, top->max_height - 1);
	move_disks(top, top->from, top->to, top->aux, 1);
	move_disks(top, top->aux, top->to, top->from, top->max_height - 1);

	/* Print the result. */
	print_towers(top);

	/* Terminate towers. */
	term_towers(top);
}

static void process(FILE *is, FILE *os)
{
	struct parameter *param;
	struct towers app;

	for (prompt(os); (param = input(is, os)) != NULL; prompt(os))
		if (param->output == NULL)
			xprintf(os, "\nPlease select the supported "
					"height of the tower.\n\n");
		else
			run(&app, param);
}

static void print_title(FILE *os)
{
	xprintf(os, "\nHow to move disks in Tower of Hanoi\n");
	xprintf(os, "===================================\n\n");
}

static void print_greeting(FILE *os)
{
	xprintf(os, "\nThank you!\n");
}

int main()
{
	FILE *is = stdin, *os = stdout;

	print_title(os);
	process(is, os);
	print_greeting(os);

	if (os != stdout)
		fclose(os);
	if (is != stdin)
		fclose(is);

	exit(EXIT_SUCCESS);
}
