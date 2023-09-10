#include "bubble.h"
#include <stdio.h>
#include <stdlib.h>


void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
	if (stats == NULL || arr == NULL)
	{
		exit(1);
	}
	//Tracks to see if a swap has been made
	int swapped;
	for(uint32_t i = 0; i < (n_elements - 1); i += 1){
		//Set to 0 every time the whole list is iterated over
		swapped = 0;
		for(uint32_t j = (n_elements - 1); j > i; j -= 1){
			//If j is less than j-1, swap j and j-1
			int comp = cmp(stats, arr[j], arr[j-1]);
			if(comp == -1){
				swap(stats, &arr[j], &arr[j-1]);
				swapped = 1;
			}
		}
		//End of no swaps occured
		if(!swapped){
			break;
		}
	}

}

