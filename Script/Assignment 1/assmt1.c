/* Assigment 1, Engineering Computation COMP 20005 Semester 2, 2016
   Written by Harfiyanto Dharma Santoso (772503), September 2016
   C Programming is fun.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AVG_INFLATION 0.0181
#define CURRENT_YEAR 2016
#define MAX_NUM 99
#define MAX_TITLE 101

int scan_list(int* num, int* year, int* earning, int* budget, char title[][MAX_TITLE]);
int calc_prj_gross(int earning, int year);
double calc_gross_ratio(int earning, int budget);
int highest_projected_index(int* earning, int* year, int count);

int 
main(int argc, char *argv[]) {

	int num[MAX_NUM], earning[MAX_NUM], budget[MAX_NUM], i, count = 0, 
		ten = 0, highest = 0;
	char title[MAX_NUM][MAX_TITLE];
	int year[MAX_NUM];
	double ratio;


	/* Reading and storing all the information in arrays, also calculating
	   the number of movies in the list.
	*/

	count = scan_list(num, year, earning, budget, title);



	/*___________________ STAGE 1 ___________________*/

	printf("Stage 1\n");
	printf("==========\n");
	printf("Movie %02d (%d) grossed in %04dm with a budget of %04dm\n", 
		num[0], year[0], earning[0], budget[0]);
	printf("Gross-budget ratio: %05.2f\n", calc_gross_ratio(earning[0], 
		budget[0]));
	printf("Projected gross: %dm\n", calc_prj_gross(earning[0], year[0]));
	printf("\n");



	/*___________________ STAGE 2 ___________________*/

	printf("Stage 2\n");
	printf("==========\n");

	/* Iterate through the array and calculate the ratio. */
	for (i = 0; i < count; i++) {

		ratio = calc_gross_ratio(earning[i], budget[i]);
		printf("Movie %02d, gross-budget ratio: %05.2f |", i + 1, ratio);
		ten = 0;
		
		/* Print '-' and '+' to represent the ratio. */
		while (ratio > 0) {

			/* Print '+'if nine '-'s have been printed. */
			if (ten == 9) {
				printf("+");
				ten = 0;
			} else {
				printf("-");
				ten++;
			}
			ratio--;

		}
		printf("\n");

	}
	printf("\n");



	/*___________________ STAGE 3 ___________________*/

	printf("Stage 3\n");
	printf("==========\n");
	printf("Total: %d movies\n", count);

	highest = highest_projected_index(earning, year, count);

	printf("Highest projected gross movie: %s\n", title[highest]);
	printf("Highest projected gross: %04dm", 
		calc_prj_gross(earning[highest], year[highest]));

	return 0;
}


/* Function that scans the list, store the data in arrays and return the 
   number of movies it scanned.
*/
int 
scan_list(int* num, int* year, int* earning, int* budget, char title[][MAX_TITLE]) {
	int count = 0;
	while (scanf("%d %d %d %d %s", num + count, year + count, earning + count, 
		budget + count, *(title + count)) == 5) {
		count++;
		if (count > MAX_NUM) {
			printf("Maximum number of movies reached.\n");
			break;
		}
	}
	return count;
}


/* Function that takes in two integer (earning and year) to calculate and
   return the projected gross.
*/
int 
calc_prj_gross(int earning, int year) {
	return (earning * pow((1 + AVG_INFLATION), (CURRENT_YEAR - year)));
}



/* Function that takes in earning and budget to calculate and
   return the Gross-budget ratio.
*/
double 
calc_gross_ratio(int earning, int budget) {
	return (double) earning / budget;
}



/* Function that takes in earning and year to determine  and return 
   the index of the entry with highest projected gross.
*/
int 
highest_projected_index(int* earning, int* year, int count) {
	int i, highest = 0;
	for (i = 1; i < count; i++) {
		if (calc_prj_gross(earning[highest], year[highest]) < 
			calc_prj_gross(earning[i], year[i])) {
			highest = i;
		}
	}
	return highest;

}