#include <stdio.h> 
#include <stdlib.h> 

#define INITIAL_SIZE 1

// base structure 
typedef struct { 
	size_t size; 
	size_t capacity; 
	double* array; 
} dynamic_array; 

void arrayInit(dynamic_array** arr_ptr) 
{ 
	dynamic_array *container; 
	container = (dynamic_array*)malloc(sizeof(dynamic_array)); 
	if(!container) { 
		printf("Memory Allocation Failed\n"); 
		exit(0); 
	} 

	container->size = 0; 
	container->capacity = INITIAL_SIZE; 
	container->array = (double *)malloc(INITIAL_SIZE * sizeof(double)); 
	if (!container->array){ 
		printf("Memory Allocation Failed\n"); 
		exit(0); 
	} 

	*arr_ptr = container; 
} 
void insertItem(dynamic_array* container, double item) 
{ 
	if (container->size == container->capacity) { 
		double *temp = container->array; 
		container->capacity <<= 1; 
		container->array = (double *)realloc(container->array, container->capacity * sizeof(double)); 
		if(!container->array) { 
			printf("Out of Memory\n"); 
			container->array = temp; 
			return; 
		} 
	} 
	container->array[container->size++] = item; 
} 
void freeArray(dynamic_array* container) 
{ 
	free(container->array); 
	free(container); 
}
