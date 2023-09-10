#include <math.h>
#include "mathlib.h"
#include <stdio.h>
#include <unistd.h>
#define OPTIONS "ascSCTl"
#define NUM_OPTIONS 7

int main(int argc, char **argv) {
	int opt = 0;
	int fall_through = 0;
	//NUM_OPTIONS is the number of command line arguments accepted
	int commands[NUM_OPTIONS] = {0};
	int num_options = NUM_OPTIONS;

	/*This iterates over all of the command line arguments, checking for an a
	creating an array of all of the other arguments, as getopt() only works once,
	and I need to iterate over them again. */
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
		case 'a':
			commands[0] = opt;
			break;

		case 's':
			commands[1] = opt;
			break;

		case 'c':
			commands[2] = opt;
			break;

		case 'S':
			commands[3] = opt;
			break;

		case 'C':
			commands[4] = opt;
			break;

		case 'T':
			commands[5] = opt;
			break;

		case 'l':
			commands[6] = opt;
			break;

		default:
			break;
		}
	}
	/*If one of the cases is a, the first command is set to a, then the
	later for loop is told to run only once, while allowing the switch
	statement inside of it to fall through, causing all tests to be run.*/
	if (commands[0] == 'a'){
		num_options = 1;
	}

	for (int i = 0; i < num_options; i += 1){
		int arg = commands[i];
		switch(arg){
			case 'a': // If the case is a, the program should fall through and run all tests
				fall_through = 1;
			case 's':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "sin", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = 0; j <= (M_PI * 2); j += (0.05 * M_PI)){
					double my_val = my_sin(j);
					double lib_val = sin(j);
					double dif = my_val - lib_val;
					//Prints results in given format
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0) //falling through is intentional, see notes on a
				{
					break;
				}
			case 'c':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "cos", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = 0; j <= (M_PI * 2); j += (0.05 * M_PI)){
					double my_val = my_cos(j);
					double lib_val = cos(j);
					double dif = my_val - lib_val;
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0)//falling through is intentional, see notes on a
				{
					break;
				}
			case 'S':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "arcsin", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = -1; j < 1; j += 0.05){
					double my_val = my_arcsin(j);
					double lib_val = asin(j);
					double dif = my_val - lib_val;
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0)//falling through is intentional, see notes on a
				{
					break;
				}
			case 'C':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "arccos", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = -1; j < 1; j += 0.05){
					double my_val = my_arccos(j);
					double lib_val = acos(j);
					double dif = my_val - lib_val;
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0)//falling through is intentional, see notes on a
				{
					break;
				}
			case 'T':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "arctan", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = 1; j < 10; j += 0.05){
					double my_val = my_arctan(j);
					double lib_val = atan(j);
					double dif = my_val - lib_val;
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0)//falling through is intentional, see notes on a
				{
					break;
				}
			case 'l':
				//Prints header first, in the given format
				printf("  %-12s %-16s %-14s %-16s\n", "x", "log", "Library", "Difference");
				printf("  %-12s %-16s %-14s %-16s\n", "-", "------", "-------", "----------");
				for (double j = 1; j < 10; j += 0.05){
					double my_val = my_log(j);
					double lib_val = log(j);
					double dif = my_val - lib_val;
					printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", j, my_val, lib_val, dif);
				}
				if (fall_through == 0)//falling through is intentional, see notes on a
				{
					break;
				}
			default:
				break;
		}
	}

	return 0;
}
