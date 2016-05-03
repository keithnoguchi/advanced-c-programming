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

static void process_scores(const int student_id, const int number_of_scores)
{
	int min, max;
	float sum;
	int score;
	int ret;
	int i;

	min = max = sum = 0;
	for (i = 0; i < number_of_scores; ++i) {
		ret = scanf("%d,", &score);
		if (ret == EOF || ret != 1) {
			break;
		}
		printf("\n%d (Grade %s)", score, score_to_letter_grade(score));
		sum += score;
		if (i == 0) {
			min = max = score;
		} else {
			if (score < min)
				min = score;
			else if (score > max)
				max = score;
		}
	}
	printf("\n\nStudent ID %d: Average score: %.2f, minimum score: %d"
			", and maximum score: %d\n",
			student_id, sum / number_of_scores, min, max);
}

static void prompt_number_of_students(void)
{
	printf("With how many students do you want to devide the scores up? ");
}

static int get_number_of_students(int *number_of_student)
{
	return scanf("%d", number_of_student);
}

int main(int argc, char *argv[])
{
	const int default_number_of_students = 4;
	const int number_of_scores = 60;
	int scores_per_student;
	int number_of_students;
	const char *filename;
	int ret;
	int i;

	/* */
	for (prompt_number_of_students();
		(ret = get_number_of_students(&number_of_students)) != 1;
		prompt_number_of_students()) {
		if (number_of_students >= 4 && number_of_students <= 6)
			break;
		printf("We only support 4, 5, or 6 students\n");
	}

	printf("Input 60 scores to process: ");
	scores_per_student = number_of_scores / number_of_students;
	for (i = 0; i < number_of_students; ++i)
		process_scores(next_student_id(), scores_per_student);
}
