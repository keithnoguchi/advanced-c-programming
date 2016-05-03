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
              for each student.  Assign the student number by calling a
              function and using a static variable initialized to 1000 and
              student number incremented to next value.  Show the student
              number along with the statistics for the scores.  Find the
              average, minimum, and maximum for all students.  Print all
              scores for each student.  Write all the output to a file.
              Assign a letter grade for each score and print.

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
 * Student number is >= 1000.
 */
static int next_student_id(void)
{
	static int student_id = 1000;
	return student_id++;
}

static int get_scores(const char *const file_name, int scores[], const int size)
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

static void print_student_id(const int student_id)
{
	printf("Student ID: %d, ", student_id);
}

static void print_average(const int scores[],
				const size_t number_of_scores)
{
	int min, max;
	float sum;
	int i;

	min = max = sum = 0;
	for (i = 0; i < number_of_scores; ++i) {
		sum += scores[i];
		if (i == 0) {
			min = max = scores[i];
		} else {
			if (scores[i] < min)
				min = scores[i];
			else if (scores[i] > max)
				max = scores[i];
		}
	}
	printf("Average score: %.2f, minimum score: %d"
			", and maximum score: %d\n",
			sum / number_of_scores, min, max);
}

/*
   Grading Policy:

   A+: >  95
   A:  == 95
   A-: >= 90
   B+: >  85
   B:  == 85
   B-: >= 80
   C+: >  75
   C:  == 75
   C-: >= 70
   D:  >= 60
   F:  <  60
*/
static char *score_to_letter_grade(const int score)
{
	if (score > 95)
		return "A+";
	else if (score == 95)
		return "A";
	else if (score >= 90)
		return "A-";
	else if (score > 85)
		return "B+";
	else if (score == 85)
		return "B";
	else if (score >= 80)
		return "B-";
	else if (score > 75)
		return "C+";
	else if (score == 75)
		return "C";
	else if (score >= 70)
		return "C-";
	else if (score >= 60)
		return "D";
	else
		return "F";
}

static void print_letter_grade(const int scores[],
				const size_t number_of_scores)
{
	static const int number_of_column = 5;
	int i;

	printf("%-30s", "Letter grade for each score: ");
	for (i = 0; i < number_of_scores; ++i) {
		printf("%d(%-2s) ", scores[i],
			score_to_letter_grade(scores[i]));
		if (((i + 1) % number_of_column) == 0)
			printf("\n%30c", ' ');
	}
}

static void usage(const char *const progname, const int ret)
{
	printf("Usage: %s <input_filename> <number_of_students>\n", progname);
	exit(ret);
}

int main(int argc, char *argv[])
{
	const char *const default_input_filename = "input.txt";
	const int default_number_of_students = 4;
	const int max_input_size = 60;
	int input_scores[max_input_size];
	int scores_per_student;
	int number_of_students;
	int number_of_scores;
	const char *filename;
	int i;

	/* Get the input filename. */
	filename = default_input_filename;
	if (argc >= 2) {
		filename = argv[1];
	} else {
		printf("No input file has been specified. ");
		printf("We'll use %s as the input file.\n", filename);
	}
	number_of_scores = get_scores(filename, input_scores, max_input_size);
	if (number_of_scores < 0) {
		/* Error happened.  Don't proceed. */
		exit(-1);
	}

	/* Get the number of students. */
	number_of_students = default_number_of_students;
	if (argc >= 3) {
		int students = atoi(argv[2]);

		if (students < 4 || students > 6) {
			fprintf(stderr,
				"Please use either 4, 5, or 6 students\n");
			usage(argv[0], -1);
		} else {
			number_of_students = students;
		}
	}

	/* Print usage output. */
	if (argc >= 4)
		usage(argv[0], 0);

	/* Print out the student statistics alnog with the letter
	 * grade of indivisual scores. */
	scores_per_student = number_of_scores / number_of_students;
	for (i = 0; i < number_of_students; ++i) {
		const int *scores = &input_scores[scores_per_student * i];
		int student_id = next_student_id();

		print_student_id(student_id);
		print_average(scores, scores_per_student);
		print_letter_grade(scores, scores_per_student);
		printf("\n");
	}
}
