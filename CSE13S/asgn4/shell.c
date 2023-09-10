#include "shell.h"
#include <stdio.h>
#include <stdlib.h>


static uint32_t gaps(uint32_t current){
	//If the current value is 1, the array is sorted, so 0 is returned to end the loop below
	if(current == 1){
		return 0;
	}
	//If the current value is <=2, the gap calculation will result in a value of 0, which will break the function. So return 1 instead
	if(current <= 2){
		current = 1;
	//Otherwise, calculate the gap
	}else{
		current *= 5;
		current /= 11;
	}

	return current;
}

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){

	if (stats == NULL || arr == NULL)
	{
		exit(1);
	}

	uint32_t initial_gap = gaps(n_elements);
	//This should iterate over all the gaps by caculating the next gap, using the previous
	for(uint32_t gap = initial_gap; gap > 0; gap = gaps(gap)){	
		//Iterate between the value of i and gap
		for(uint32_t i = gap; i < n_elements; i+=1){
			uint32_t j = i;
			uint32_t temp = move(stats, arr[i]);
			//The first check in the while loop checks to see if j can be moved any further forward
			//If it can, it then compares element j and element j-gap, swapping the two if element j <= element j - gap
			while(j >= gap && cmp(stats, temp, arr[j-gap]) == -1){
				//This moves once to read it, and again to write it
				arr[j] = move(stats, move(stats, arr[j-gap]));
				j -= gap;
			}
			arr[j] = move(stats, move(stats, temp));
		}
	}

}


