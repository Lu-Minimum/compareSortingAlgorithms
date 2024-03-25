#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

/*
Testing various actions for how fast they are (on my computer) results:
	Defining and initializing a new variable is slower than setting a value to an existing variable
	Arrays are faster to access than pointers
	For loops are faster than while loops
	Getting a value from an array is faster than setting one
	A comparison is faster than getting a value from an array
	Setting a value in an array vs in a variable showed no significant difference
	Getting a value from an array vs from a variable showed no significant difference
These results are just to help me test things in a reasonable amount of time
*/

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	//Deal with null or too short list.
	if (arr==NULL||n<=1) {
		return;
	}

	int temp, i, j;

	//Loop to convert array to max heap
	//O(nlogn)
	for (i = 1; i<n; i++) {
		//i is current size of heap

		//loop to perlocate up
		for (j=i; j>0 && arr[j]>arr[(j-1)/2]; j=(j-1)/2) {
			temp = arr[j];
			arr[j] = arr[(j-1)/2];
			arr[(j-1)/2] = temp;
		}
	}

	//Loop to create sorted array using heap
	//O(nlogn)
	for (i = n; i>1; i--) {
		//i is current size of heap

		//swap last item in heap with first
		temp = arr[i-1];
		arr[i-1] = arr[0];
		arr[0] = temp;

		if (i>2) {
			//loop to perlocate down
			//O(logn)
			j = 0;
			while (j <= (i-3)/2) {
				if (2*j+2>=i-1) {
					if (arr[j] < arr[2*j+1]) {
						temp = arr[2*j+1];
						arr[2*j+1] = arr[j];
						arr[j] = temp;
					}
					break;
				}
				if (arr[j] >= arr[2*j+1] && arr[j] >= arr[2*j+2]) {
					break;
				}
				if (arr[2*j+1] >= arr[2*j+2]) {
					temp = arr[2*j+1];
					arr[2*j+1] = arr[j];
					arr[j] = temp;
					j = 2*j+1;
				} else {
					temp = arr[2*j+2];
					arr[2*j+2] = arr[j];
					arr[j] = temp;
					j = 2*j+2;
				}
			}
		}
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//Deal with null or too short list.
	if (pData==NULL||r-l<=0) {
		return;
	}

	//Check for only 2 elements
	if (r-l==1) {
		if (pData[r]<pData[l]) {
			int temp = pData[r];
			pData[r] = pData[l];
			pData[l] = temp;
		}
		return;
	}

	//Recursively sort sublists
	int mid = (l+r)/2;
	//Recursion depth is O(logn)
	mergeSort(pData, l, mid);
	mergeSort(pData, mid+1, r);

	int* tempArr = (int*) Alloc((1+r-l)*sizeof(int));
	//c1 and c2 are positions in left and right sublists
	int c1 = l, c2 = mid+1, i;

	//Merge sublists
	//O(n)
	for (i = 0; i < (1+r-l); i++) {
		if (c2>r) {
			tempArr[i] = pData[c1];
			c1++;
			continue;
		}
		if (c1>mid || pData[c2] < pData[c1]) {
			tempArr[i] = pData[c2];
			c2++;
		} else {
			tempArr[i] = pData[c1];
			c1++;
		}
	}

	//Copy the temp arr back to the data
	//O(n)
	for (i = 0; i < (1+r-l); i++) {
		pData[l+i] = tempArr[i];
	}

	//Dealloc
	DeAlloc(tempArr);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	//Deal with null or too short list.
	if (pData==NULL||n<=1) {
		return;
	}

	int temp, i, j;

	//O(n^2)
	for (i=1; i<n; i++) {
		//i is the number of sorted elements or index of the first unsorted element

		//Insert next unsorted element into sorted elements
		for (j=i; j>0; j--) {
			if (pData[j] < pData[j-1]) {
				temp = pData[j];
				pData[j] = pData[j-1];
				pData[j-1] = temp;
			} else {
				break;
			}
		}
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	//Deal with null or too short list.
	if (pData==NULL||n<=1) {
		return;
	}

	int temp, i, j;

	//O(n^2)
	for (i = n-1; i > 0; i--) {
		//i is 1 less than the amount of elements that need to be sorted
		for (j = 0; j < i; j++) {
			if (pData[j]>pData[j+1]) {
				temp = pData[j+1];
				pData[j+1] = pData[j];
				pData[j] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	//Deal with null or too short list.
	if (pData==NULL||n<=1) {
		return;
	}

	int temp, i, j, minIndex;

	//O(n^2)
	for (i=0; i<n-1; i++) {
		//i is the number of sorted elements or index of the first unsorted element

		minIndex = i;

		//Loop to find minimum
		//O(n)
		for (j=i+1; j<n; j++) {
			if (pData[j]<pData[minIndex]) {
				minIndex = j;
			}
		}

		//Swap first unsorted with minimum
		//O(1)
		if (minIndex!=i) {
			temp = pData[minIndex];
			pData[minIndex] = pData[i];
			pData[i] = temp;
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		//Check for null
		if (*ppData==NULL) {
			return 0;
		}

		//Get data
		//O(n)
		for (int i = 0; i < dataSz; i++) {
			fscanf(inFile,"%d",(*ppData)+i);
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
