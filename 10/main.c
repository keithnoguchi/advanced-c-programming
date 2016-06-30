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

/* Pole with the height and the pointer to the top of the disk. */
struct pole {
	int height;
	struct disk *top;
};

/* Three towers of Hanoi. */
struct poles {
	struct pole *from;
	struct pole *aux;
	struct pole *to;
};

typedef struct supported_height {
	const int height;
	const char *const output;
} supported_level_t;

/* Preconfigured level specific information. */
static struct supported_height supported_levels[] = {
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

static struct pole *new_pole()
{
	struct pole *pole;

	pole = malloc(sizeof(struct pole));
	assert(pole != NULL);
	pole->height = 0;
	pole->top = NULL;

	return pole;
}

static void init_poles(struct poles *poles, const size_t height)
{
	poles->from = new_pole();
	assert(poles->from != NULL);
	poles->aux = new_pole();
	assert(poles->aux != NULL);
	poles->to = new_pole();
	assert(poles->to != NULL);
}

static void prompt(FILE *os)
{
	int i;

	fprintf(os, "Tell me the height of the 'from' tower, e.g. ");
	for (i = 0; supported_levels[i].output != NULL; i++) {
		xprintf(os, "%d, ", supported_levels[i].height);
	}
	fprintf(os, "or -1 to quit? ");
}

static supported_level_t *const input(FILE *is, FILE *os)
{
	supported_level_t *level;
	int number;
	char c;
	int i;
	int ret;

	ret = fscanf(is, "%d%c", &number, &c);
	xprintf(os, "%d\n", number);

	if (ret != 2 || number == -1)
		return NULL;

	for (i = 0; supported_levels[i].output != NULL; i++)
		if (number == supported_levels[i].height)
			return &supported_levels[i];

	return &supported_levels[i];
}

static void process(FILE *is, FILE *os)
{
	supported_level_t *config;
	struct poles poles;

	for (prompt(os); (config = input(is, os)) != NULL; prompt(os)) {
		if (config->output == NULL) {
			xprintf(os, "\nPlease select the supported "
					"height of the tower.\n\n");
			continue;
		}
		xprintf(os, "Your tower is this(%d) tall!\n", config->height);
		init_poles(&poles, config->height);
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
