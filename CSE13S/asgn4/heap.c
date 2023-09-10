#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

static uint32_t left_child(uint32_t n){
	return ((2 * n) + 1);
}

static uint32_t right_child(uint32_t n){
	return ((2 * n) + 2);
}

static uint32_t parent(uint32_t n){
	return (n - 1) / 2;
}

static void up_heap(uint32_t *arr, uint32_t n, Stats *stats){
	//Compares arr[n] and it's parent and swaps them until the parent is smaller than arr[n]
	while(n > 0 && cmp(stats, arr[n], arr[parent(n)]) == -1){
		swap(stats, &arr[n], &arr[parent(n)]);
		n = parent(n);
	}
}

static void build_heap(uint32_t *arr, uint32_t *heap, uint32_t n_elements, Stats *stats){
	//Just runs up heap on all of the elements of arr, fairly simple
	for (uint32_t n = 0; n < n_elements; n += 1){
		//One move for read, another for write
		heap[n] = move(stats, move(stats, arr[n]));
		up_heap(heap, n, stats);
	}
}

static void down_heap(uint32_t *arr, uint32_t heap_size, Stats *stats){
	uint32_t n = 0;
	uint32_t smaller;
	//Checks to make sure that there is a left child in the heap
	while(left_child(n) < heap_size){
		//If right child == heap_size, there is no right child, so we know left child will be the smaller
		if(right_child(n) == heap_size){
			smaller = left_child(n);
		}
		else{
			//Determine the smaller child
			if(cmp(stats, arr[left_child(n)], arr[right_child(n)]) == -1){
				smaller = left_child(n);
			}else{
				smaller = right_child(n);
			}
		}
		//If the smaller child is smaller than the parent, then both childern and the parent are in the right place, so exit out of the loop
		if(cmp(stats, arr[n], arr[smaller]) == -1){
			break;
		}
		//Otherwise, they're in the wrong place, so swap the two
		swap(stats, &arr[n], &arr[smaller]);
		n = smaller;
	}
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
	if (stats == NULL || arr == NULL)
	{
		exit(1);
	}

	uint32_t *heap = calloc(n_elements, sizeof(uint32_t));

	//Make a heap based on the passed array
	build_heap(arr, heap, n_elements, stats);

	for(uint32_t n = 0; n < n_elements; n += 1){
		//One move for read, another for write
		//Move the smallest value, found at the top of the heap, into the next value of the sorted array
		arr[n] = move(stats, move(stats, heap[0]));
		//One move for read, another for write
		//Move one of the largest childeren into heap[0], then fix the heap
		heap[0] = move(stats, move(stats, heap[(n_elements - n - 1)]));
		down_heap(heap, (n_elements - n), stats);
	}

	free(heap);
}

