/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 10

   Topic: Tower of Hanoi, the power of recursion

   file name: main.c

   Date: June 28th, 2016

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

/* Supported number of disks, and the corresponding output filename. */
typedef enum difficulty_level {
	LOW = 0,
	MEDIUM,
	HIGH,
	LEVEL_MAX
} difficulty_level_t;

typedef struct supported_level {
	difficulty_level_t level;
	const int disks;
	const char *const level_str;
	const char *const filename;
} supported_level_t;

/* Preconfigured level specific information. */
static supported_level_t supported_levels[] = {
	{
		.level = LOW,
		.level_str = "low",
		.disks = 1,
		.filename = "output1.txt"
	},
	{
		.level = MEDIUM,
		.level_str = "medium",
		.disks = 2,
		.filename = "output2.txt"
	},
	{
		.level = HIGH,
		.level_str = "high",
		.disks = 3,
		.filename = "output3.txt"
	},
	{
		.level = LEVEL_MAX,
		.level_str = "invalid",
		.disks = -1,
		.filename = NULL
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

static void prompt(FILE *os)
{
	int i;

	fprintf(os, "How many disks do you want, ");
	for (i = LOW; i < LEVEL_MAX; i++) {
		fprintf(os, "%d, ", supported_levels[i].disks);
	}
	fprintf(os, "or -1 to quit? ");
}

static supported_level_t *const input(FILE *is, FILE *os)
{
	supported_level_t *level;
	difficulty_level_t i;
	int number;
	char c;
	int ret;

	ret = fscanf(is, "%d%c", &number, &c);
	xprintf(os, "%d\n", number);

	if (ret != 2 || number == -1)
		return NULL;

	for (i = LOW; i < LEVEL_MAX; i++)
		if (number == supported_levels[i].disks)
			return &supported_levels[i];

	return &supported_levels[i];
}

static void process(FILE *is, FILE *os)
{
	supported_level_t *config;

	for (prompt(os); (config = input(is, os)) != NULL; prompt(os)) {
		if (config->level == LEVEL_MAX)
			xprintf(os, "\nPlease select the supported "
					"number of disks.\n\n");
	}
}

int main()
{
	FILE *is = stdin, *os = stdout;

	xprintf(os, "\nGame: Tower of Hanoi\n");
	xprintf(os, "==============\n\n");

	process(is, os);
	xprintf(os, "\nThank you!\n");
out:
	if (os != stdout)
		fclose(os);
	if (is != stdin)
		fclose(is);

	exit(EXIT_SUCCESS);
}
