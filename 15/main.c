/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 15

   Topic: Hash

   file name: main.c

   Date: June 30th, 2016

   Objective: Implement a hash table of size 15.  Use the Separate
              Chaining for collisions.

              Give your input file of 100 records with keys.  Use
              the structure of {key, record, next} for the linked
              nodes.
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static const int input_file_generation_retry = 3;
static const int total_number_of_random_keys = 100;
static const int random_key_max = 1000;

static void generate_random_file(const char *const output)
{
	FILE *os;
	int value;
	int i;

	srand(time(NULL));

	os = fopen(output, "w");
	if (os == NULL)
		return;

	for (i = 0; i < total_number_of_random_keys; i++) {
		value = rand() % random_key_max;
		fprintf(os, "%d ", value);
	}

	fclose(os);
}

int main()
{
	int retry = input_file_generation_retry;
	const char *const input = "input.txt";
	const char *const output = "output.txt";
	FILE *is = NULL, *os = NULL;

	while ((is = fopen(input, "r")) == NULL) {
		/* Let's generate a random numbers. */
		generate_random_file(input);
		if (--retry == 0)
			goto out;
	}

	os = fopen(output, "w");
	if (os == NULL)
		goto out;

	printf("Here you go!\n");

out:
	if (is != NULL)
		fclose(is);
	if (os != NULL)
		fclose(os);

	exit(EXIT_SUCCESS);
}
