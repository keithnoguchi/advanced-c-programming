/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 2

   Topic: Print average, maximum, and minimum

   file name: main.c

   Date: April 20th, 2016

   Objective: Read one student scores from different exams as input from
              standard input, the scores are as follows:

              Scores: 91, 92, 85, 68, 87, 75, 89, 97, 79, 65, 88, 72,
	              81, 94, 90

	      Print the average, maximum and minimum of all inputs received.
	      Print each of the input as it is read one by one. Do not call
	      any functions and do not use any arrays.

   Example:
   $ ./a.out
   91, 92, 85, 68, 87, 75, 89, 97, 79, 65, 88, 72, 81, 94, 90
   ^D
   Maximum: 97, minimum: 65, and the average: 83.53
   $ ./a.out

 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
	char buf[BUFSIZ];
	int max, min, count;
	float sum;
	int val;
	int c;
	int i;

	max = min = sum = count = 0;
	i = 0;
	while ((c = getchar()) != EOF) {
		if (isdigit(c)) {
			/* Store the buffer for the later conversion. */
			buf[i++] = c;
			/* Buffer overflow check. */
			if (i >= sizeof(buf))
				i = 0;
		} else {
			if (i > 0) {
				/* Got the score. */
				buf[i] = '\0';
				val = atoi(buf);
				sum += val;
				count += 1;
				if (count == 1) {
					max = min = val;
				} else {
					if (val > max)
						max = val;
					if (val < min)
						min = val;
				}
				/* Let's take care of next score. */
				i = 0;
			}
		}
	}

	if (count > 0)
		printf("Maximum: %d, minimum: %d, and the average: %.2f\n",
			max, min, sum / count);
	else
		printf("Please provide at least one score, as 1, 2, 3\n");
}
