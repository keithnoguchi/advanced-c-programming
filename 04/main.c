/*
   University of California Extension, Santa Cruz

   Advanced C Programming

   Instructor: Rajainder A.
   Author: Kei Nohguchi
   Assignment Number: 4

   Topic: Print min, max, and average for the multiple student
          with the grading capability.

   file name: main.c

   Date: April 25th, 2016

   Objective: Read the student scores for 4 to 6 students from different
              exams as input from a input file, the scores as follows:
	      91, 92, 85, 58, 87, 75, 89, 97, 79, 65, 88, 72, 81, 94, 90, 61,
	      72, 75, 68, 77, 75, 49, 87, 79, 65, 64, 62, 51, 44, 70, 81, 72,
	      85, 78, 77, 75, 79, 87, 69, 55, 88, 62, 71, 74, 80, 71, 62, 85,
	      68, 87, 75, 89, 97, 79, 65, 48, 72, 61, 64, 90

	      Call functions to find the average, minimum, and maximum score
	      for each student.  Call a function to assign the student number
	      using a static variable for the student number which will
	      automatically increment whenever the function is called.
	      Show the student number along with the statistics for the
	      scores.  Find the average, minimum, and maximum for all
	      students.  Print all scores for each student.  Write all the
	      output to a file.  Assign a letter grade for each score
	      and print.

	      Grading Policy:

	      A+: >  95
              A:     95
	      A-: >= 90
	      B+: >  85
              B:     85
	      B-: >= 80
	      C+: >  75
              C:     75
	      C-: >= 70
              D:  >= 60
              F:  <  60

*/

#include <stdio.h>

/* Return the next student number available.
 *
 * Student number is > 0.
 */
static int next_student_number(void)
{
	static int number = 0;
	return ++number;
}

int main()
{
	int i;

	for (i = 0; i < 10; ++i)
		printf("Student number: %d\n", next_student_number());
}
