#include "quick.h"
#include <stdio.h>
#include <stdlib.h>

#define SMALL 8

static uint32_t gaps(uint32_t current){
	if(current == 1){
		return 0;
	}

	if(current <= 2){
		current = 1;
	}else{
		current *= 5;
		current /= 11;
	}

	return current;
}
//Shell sort is used here for smaller arrays, as quicksort performs poorly with them. See shell.c for comments
static void altered_shell_sort(Stats *stats, uint32_t *arr, uint32_t start, uint32_t stop){

	if (stats == NULL || arr == NULL || stop - start <= 1)
	{
		return;
	}

	uint32_t initial_gap = gaps(stop - start);
	for(uint32_t gap = initial_gap; gap > 0; gap = gaps(gap)){	
		for(uint32_t i = start + gap; i < stop; i+=1){
			uint32_t j = i;
			uint32_t temp = move(stats, arr[i]);
			while(j >= start + gap && cmp(stats, temp, arr[j-gap]) == -1){
				//This moves once to read it, and again to write it
				arr[j] = move(stats, move(stats, arr[j-gap]));
				j -= gap;
			}
			arr[j] = move(stats, move(stats, temp));
		}
	}

}

static void actual_quick_sort(Stats *stats, uint32_t *arr, uint32_t start, uint32_t stop){
	if(stop - start <= 8){
		altered_shell_sort(stats, arr, start, stop);
		return;
	}

	//By the end of the first for loop, mid start will be where the items less than the pivot end
	//Mid buffer will be the element of the first item equal to pivot.
	uint32_t mid_start = start;
	uint32_t mid_buffer = stop;
	uint32_t mid_end;

	uint32_t pivot = (arr[start] + arr[(start + stop)/2] + arr[stop - 1])/3;
	//For the three reads used to calculate pivot
	stats->moves += 3;

	//Iterate between start and mid buffer
	for(uint32_t i = start; i < mid_buffer; i += 1){
		int temp = cmp(stats, arr[i], pivot);
		//If element i is less than the pivot, then swap mid start and i, then increment mid start
		if(temp == -1){
			if(i != mid_start){
				swap(stats, &arr[i], &arr[mid_start]);
			}
			mid_start += 1;
		//If element i equals the pivot, swap i with mid buffer and decreace both
		//This should create a segment of all the values equal to the pivot at the end of the array.
		}else if(temp == 0){
			swap(stats, &arr[i], &arr[mid_buffer-1]);
			i -= 1;
			mid_buffer -= 1;
		}

	}

	mid_end = mid_start;
	//Move all the items equal to the pivot at the end back to the middle
	if (mid_buffer != stop)	{
		for (uint32_t i = mid_buffer; i < stop; i += 1){
			swap(stats, &arr[i], &arr[mid_end]);
			mid_end += 1;
		}
	}
	//Call quick sort on all the values less than the pivot, and all values greater than the pivot
	actual_quick_sort(stats, arr, start, mid_start);
	actual_quick_sort(stats, arr, mid_end, stop);
}

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){

	if (stats == NULL || arr == NULL)
	{
		exit(1);
	}
	//To make quick sort, I want to pass the initial element and the last element to be sorted, so my implementation can be in line
	//However, I can't change the parameters of quick sort.
	//As such, I immediately call actual quick sort with the parameters I want, which will then sort the array
	actual_quick_sort(stats, arr, 0, n_elements);

}

