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
#include <stdlib.h>

/* Return the next student number available.
 *
 * Student number is > 0.
 */
static int next_student_id(void)
{
	static int student_id = 0;
	return ++student_id;
}

static int get_scores(const char *file_name, int scores[], const int size)
{
	FILE *fp;
	int value;
	int ret;
	int i;

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open %s\n", file_name);
		return -1;
	}
	for (i = 0; i < size; ++i) {
		ret = fscanf(fp, "%d,", &value);
		if (ret == EOF || ret != 1) {
			break;
		}
		scores[i] = value;
	}
	fclose(fp);

	return i;
}

static void print_scores(const int student_id, const int scores[],
		         const size_t number_of_scores)
{
	const int number_of_column = 16;
	int i;

	printf("Student ID: %d\n", student_id);
	for (i = 0; i < number_of_scores; ++i) {
		printf("%d ", scores[i]);
		if ((i + 1) % number_of_column == 0)
			printf("\n");
	}
	printf("\n");
}


int main(int argc, char *argv[])
{
	char *file_name = "input.txt";
	const int size = 60;
	int students = 4;
	int scores[size];
	int number_of_scores;
	int scores_per_students;
	int ret;
	int i;

	if (argc >= 2)
		file_name = argv[1];
	if (argc >= 3)
		students = atoi(argv[2]);
	if (students < 4 || students > 6) {
		fprintf(stderr, "We only support students 4, 5, or 6\n");
		exit(-1);
	}

	number_of_scores = get_scores(file_name, scores, size);

	for (i = 0; i < students; ++i) {
		int student_id = next_student_id();
		scores_per_students = number_of_scores / students;
		print_scores(student_id, &scores[scores_per_students * i],
				scores_per_students);
	}
}
