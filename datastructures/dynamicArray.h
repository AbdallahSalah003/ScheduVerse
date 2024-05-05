#include <stdio.h>
#include <stdlib.h>
#include "../processData.h"
#define INITIAL_SIZE 1

// base structure
typedef struct
{
	size_t size;
	size_t capacity;
	double *array;
} dynamic_array;

typedef struct
{
	size_t size;
	size_t capacity;
	ProcessData **array;
} dynamic_process_array;

void arrayInit(dynamic_array **arr_ptr)
{
	dynamic_array *container;
	container = (dynamic_array *)malloc(sizeof(dynamic_array));
	if (!container)
	{
		printf("Memory Allocation Failed\n");
		exit(0);
	}

	container->size = 0;
	container->capacity = INITIAL_SIZE;
	container->array = (double *)malloc(INITIAL_SIZE * sizeof(double));
	if (!container->array)
	{
		printf("Memory Allocation Failed\n");
		exit(0);
	}

	*arr_ptr = container;
}

void processArrayCheckInit(dynamic_process_array **arr_ptr)
{	
	if((*arr_ptr)->array) return;
	dynamic_process_array *container;
	container = (dynamic_process_array *)malloc(sizeof(dynamic_process_array));
	if (!container)
	{
		printf("Memory Allocation Failed\n");
		exit(0);
	}

	container->size = 0;
	container->capacity = INITIAL_SIZE;
	container->array = (ProcessData **)malloc(INITIAL_SIZE * sizeof(ProcessData*));
	if (!container->array)
	{
		printf("Memory Allocation Failed\n");
		exit(0);
	}

	*arr_ptr = container;
}


void insertItem(dynamic_array *container, double item)
{
	if (container->size == container->capacity)
	{
		double *temp = container->array;
		container->capacity <<= 1;
		container->array = (double *)realloc(container->array, container->capacity * sizeof(double));
		if (!container->array)
		{
			printf("Out of Memory\n");
			container->array = temp;
			return;
		}
	}
	container->array[container->size++] = item;
}
void addProcessToWaitingOnMemory(dynamic_process_array *container, ProcessData* process)
{
	if (container->size == container->capacity)
	{
		ProcessData **temp = container->array;
		container->capacity <<= 1;
		container->array = (ProcessData **)realloc(container->array, container->capacity * sizeof(ProcessData*));
		if (!container->array)
		{
			printf("Out of Memory\n");
			container->array = temp;
			return;
		}
	}
	container->array[container->size++] = process;
}
void freeArray(dynamic_array *container)
{
	free(container->array);
	free(container);
}
void freeMemoryOverflow(dynamic_process_array* container){
	free(container->array);
	free(container);
}