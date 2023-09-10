#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "mtrand.h"
#include "bubble.h"
#include "quick.h"
#include "shell.h"
#include "heap.h"
#include "set.h"
#include <inttypes.h>
#include <errno.h>

#define OPTIONS "asbqhr:n:p:H"
#define SHELL 5
#define BUBBLE 2
#define QUICK 4
#define HEAP 3
#define ALL 6
#define SEED 7
#define SIZE 8
#define PRINT 9
#define ERROR 10
#define HELP 11

//Throughout this program there are blocks of commented out code. Above them will be a comment explaining what figures they were used to create
//alongside the included graphing.sh script and graphing_no_buble.sh script.
//graphing.sh was used to create all figures except 4 and 5, which graphing_no_bubble.sh was used for.


// This section was used to create figures 8, 9, and 10 by sorting the array into reverse order
/*
static void reverse_bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
	if (stats == NULL || arr == NULL)
	{
		return;
	}
	int swapped;
	for(uint32_t i = 0; i < (n_elements - 1); i += 1){
		swapped = 0;
		for(uint32_t j = (n_elements - 1); j > i; j -= 1){
			int comp = cmp(stats, arr[j], arr[j-1]);
			if(comp == 1){
				swap(stats, &arr[j], &arr[j-1]);
				swapped = 1;
			}
		}
		if(!swapped){
			break;
		}
	}
}
*/


int main(int argc, char **argv){

	uint64_t seed = 13371453;
	uint64_t size = 100;
	uint64_t print = 100;
	
	Set set = 0;
	//Set will be used to track the command line arguments
	//5 means shell sort was called
	//2 means bubble sort was called
	//5 means quick sort
	//3, heap sort
	//6, all sorts
	//7, the seed was changed
	//8, the size of the array was changed
	//9, the number of elements to be printed was changed
	//10, there was an error, so the program should return 1 and print the program usage
	//11, H was called so we should return 0 and print the program usage

	int opt = 0;
	char *endptr;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1){
		switch (opt){
			case 'a':
				//Case a adds 0-4, as 0-3 corrispond to all of the algorithms, and 4 to the fact that all sorting methods was called.
				for(int i = BUBBLE; i <= ALL; i+=1){
					set = set_insert(set, i);
				}
				break;
			case 's':
				set = set_insert(set, SHELL);
				break;
			case 'b':
				set = set_insert(set, BUBBLE);
				break;
			case 'q':
				set = set_insert(set, QUICK);
				break;
			case 'h':
				set = set_insert(set, HEAP);
				break;
			case 'r':
				set = set_insert(set, SEED);
				errno = 0;
				seed = strtoul(optarg, &endptr, 10);
				//If endptr != \0, then the passed string contained values that were not numbers. As such, error
				//If errno !=0, then the passed string was out of bounds, once again causing an error.
				if (*endptr != '\0' || errno != 0){
					set = set_insert(set, ERROR);
				}
				break;
			case 'n':
				set = set_insert(set, SIZE);
				size = strtoul(optarg, &endptr, 10);
				//See case r
				//The limits of the size range from 250 million to 1. So if stroul gets a value outside of those bounds, error
				if (size < 1 || size > 250000000 || *endptr != '\0'){
					set = set_insert(set, ERROR);
				}
				break;
			case 'p':
				set = set_insert(set, PRINT);
				errno = 0;
				print = strtoul(optarg, &endptr, 10);
				//See case r
				if (*endptr != '\0' || errno != 0){
					set = set_insert(set, ERROR);
				}
				break;
			case 'H':
				set = set_insert(set, HELP);
				break;
			default:
				set = set_insert(set, ERROR);
				break;
		}
	}

	//Creates two equal length arrays
	uint32_t *start_arr = calloc(size, sizeof(uint32_t));
	uint32_t *arr = calloc(size, sizeof(uint32_t));

	//If 10 is in the set, there was an error, so we print program usage, then return 1.
	//If start_arr or arr are null, there was also an error
	if(start_arr == NULL || arr == NULL || set_member(set, ERROR) || set_member(set, HELP)){
		printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   -a              Enable all sorts.\n   -b              Enable Bubble Sort.\n   -h              Enable Heap Sort.\n   -q              Enable Quick Sort.\n   -s              Enable Shell Sort.\n   -n length       Specify number of array elements (default: 100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r seed         Specify random seed (default: 13371453).\n");
		free(start_arr);
		free(arr);
		if (set_member(set, HELP)){
			return 0;
		}
		return 1;
	}

	//Creates an array of the functions to be used, making calling them much easier

	void (*function_pointers[])(Stats *stats, uint32_t *arr, uint32_t n_elements) = {&bubble_sort, &heap_sort, &quick_sort, &shell_sort};

	//Sets print to size, if more elements were to be printed than exisit
	if(print > size){
		print = size;
	}

	mtrand_seed(seed);

	//Generate an array of random values
	for(uint32_t n = 0; n < size; n += 1){
		//This should be 2 0s and 30 1s in a row, bit masking the result of mtrand_rand64 to 30 bits.
		uint32_t max = 1073741823;
		uint64_t test = mtrand_rand64();
		uint32_t test2 = test & max;
		start_arr[n] = test2;
	}

	Stats holder = {0, 0};

	//The names array will need to be commented out if using a script to create graphs
	char *names[] = {"Bubble Sort", "Heap Sort", "Quick Sort", "Shell Sort"};

	//This should be used to create figures 6, 7, 8, 9, and 10
	//Stats throwaway = {0 ,0};

	//This should be used for figures 6 and 7
	//function_pointers[2](&throwaway, start_arr, size);

	//This should be used for figures 8, 9, and 10
	//reverse_bubble_sort(&throwaway, start_arr, size);

	//Iterate over i = 0-3
	for (int i = 0; i < 4; i += 1){
		if(set_member(set, (i+2))){
			reset(&holder);
			//Move the values of the starting array into the array to be sorted
			for(uint64_t b = 0; b < size; b += 1){
				arr[b] = start_arr[b];
			}
			//Sort using the pointer corrisponding to i
			function_pointers[i](&holder, arr, size);

			//This print statement should be commented out if using graphing scripts
			printf("%s, %lu elements, %lu moves, %lu compares\n", names[i], size, holder.moves, holder.compares);

			//The below code was used for figures 1, 2, 4, 6, and 9
			//It simply outputs the size followed by the # of compares, which requires no parsing in bash to use in gnuplot
			//printf("%lu %lu\n", size, holder.compares);

			//The below code was used for figures 3, 5, 7, and 8
			//It simply outputs the size followed by the # of moves, which requires no parsing in bash to use in gnuplot
			//printf("%lu %lu\n", size, holder.moves);
			uint64_t iterator = 0;
			while(iterator < print){
				//Prints a newline every 5 items in a column
				for(int j = 0; j < 5; j+=1){
						if (iterator < print){
						printf("%13" PRIu32, arr[iterator]);
						iterator += 1;
					}
				}
				printf("\n");
			}
		}
	}

	free(start_arr);
	free(arr);
	return 0;
}

