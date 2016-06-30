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
#include <assert.h>

/* Disk, which points the next disk below. */
struct disk {
	const int size;
	const char *const name;
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

/* Supported parameters. */
static struct parameter {
	const int height;
	const char *const output;
} supported_params[] = {
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
	{
		.height = -1,
		.output = NULL
	}
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

static struct tower *new_tower()
{
	struct tower *tower;

	tower = malloc(sizeof(struct tower));
	assert(tower != NULL);
	tower->height = 0;
	tower->top = NULL;

	return tower;
}

static void init_towers(struct towers *top, const size_t height, FILE *os)
{
	top->os = os;
	top->max_height = height;
	top->from = new_tower();
	assert(top->from != NULL);
	top->aux = new_tower();
	assert(top->aux != NULL);
	top->to = new_tower();
	assert(top->to != NULL);
}

static void print_towers(const struct towers *top)
{
	xprintf(top->os, "Your tower is this(%d) tall!\n", top->max_height);
}

static void prompt(FILE *os)
{
	int i;

	fprintf(os, "Tell me the height of the 'from' tower, e.g. ");
	for (i = 0; supported_params[i].output != NULL; i++) {
		xprintf(os, "%d, ", supported_params[i].height);
	}
	fprintf(os, "or -1 to quit? ");
}

static struct parameter *const input(FILE *is, FILE *os)
{
	struct parameter *level;
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

static void process(FILE *is, FILE *os)
{
	struct parameter *param;
	struct towers app;

	for (prompt(os); (param = input(is, os)) != NULL; prompt(os)) {
		if (param->output == NULL) {
			xprintf(os, "\nPlease select the supported "
					"height of the tower.\n\n");
			continue;
		}
		init_towers(&app, param->height, os);
		print_towers(&app);
	}
}

int main()
{
	FILE *is = stdin, *os = stdout;

	xprintf(os, "\nHow to move disks in Tower of Hanoi\n");
	xprintf(os, "===================================\n\n");

	process(is, os);
	xprintf(os, "\nThank you!\n");
out:
	if (os != stdout)
		fclose(os);
	if (is != stdin)
		fclose(is);

	exit(EXIT_SUCCESS);
}
