#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <unistd.h>
#include <string.h>

void static isMemCleared()
{
	int* x1 = malloc(4088);
	free(x1);
	return;
}


int getTime(){
	struct timeval tv;
    gettimeofday(&tv,NULL);
	return tv.tv_usec;
}


int test1() {
    int startTime = getTime();
	for(int i=0; i<120; i++){
           	char* charptr = malloc(1);
        	free(charptr);
        }

	int finalTime = getTime();
	isMemCleared();
	return abs(finalTime-startTime);
}

int test2() {
    int startTime = getTime();
	char* ptrArr [120]; //stores 120 ptrs
	for (int i=0;i<120;i++){
		ptrArr[i]=malloc(1); //allocate 1 byte and store address in array
	}
	for (int i=0;i<120;i++){
		free(ptrArr[i]); //free the array
	}
	int finalTime = getTime();
	isMemCleared();
	return abs(finalTime-startTime);
}

int test3() {
    int startTime = getTime();

	char * ptrArray[120];
	int occupiedMem[120] = {0};
	int currPtr = 0;

	for(int counter=0; counter<120; counter++) {
		if((currPtr<120&&rand() % 2 == 0)||currPtr==0)
		{

			ptrArray[currPtr] = malloc(1);
			occupiedMem[currPtr] = 1;
			currPtr++;
        	}
		else
		{
			currPtr--;

			free(ptrArray[currPtr]);
			occupiedMem[currPtr]=0;
        	}
    	}
	for(int counter=0;counter<120;counter++) {
		if(occupiedMem[counter] == 1) {
			free(ptrArray[counter]);
		}
	}
	int endTime = getTime();
	isMemCleared();
	return abs(endTime-startTime);
}

//mallocs half and half each iteration
int test4()
{
	int startTime = getTime();
	int* arr1;
	int* arr2;
	for(int i=0; i<50; i++){
		arr1 = malloc(2040);
		arr2 = malloc(2040);
		free(arr1);
		free(arr2);
	}
	int endTime = getTime();
    isMemCleared();
	return abs(endTime-startTime);
}


//frees even then odd
int test5()
{
	
	int startTime = getTime();
    char* ptrArr[240];
    for (int i = 0; i < 240; i++) {
        ptrArr[i] = malloc(1);
        if (i % 2 == 0) {
            free(ptrArr[i]);
        }
    }
    for (int i = 0; i < 240; i++) {
        if (i % 2 != 0) {
            free(ptrArr[i]);
        }
    }
    int endTime = getTime();
    isMemCleared();
		return abs(endTime-startTime);

}


int main()
{
	//test1
	int sum=0;
	for(int i=0;i<50;i++){
		sum+=test1();
	}
	double avg=sum/50.0;
	printf("The average time taken to run test 1 was %f ms\n",avg);


	
	//test2
	sum = 0;
	for(int i=0;i<50;i++){
		sum+=test2();
	}
	avg=sum/50.0;
        printf("The average time taken to run test 2 was %f ms\n",avg);


	//test3
    sum=0;
    for(int i=0;i<50;i++){
            sum+=test3();
    	}
     avg=sum/50.0;
    printf("The average time taken to run test 3 was %f ms\n",avg);


	//test4
	sum=0;
    for(int i=0;i<50;i++){
            sum+=test4();
        }
    avg=sum/50.0;
    printf("The average time taken to run test 4 was %f ms\n",avg);

	//test5
	
	sum=0;
	for(int i=0;i<50;i++){
			sum+=test5();
	}
	avg=sum/50.0;
	printf("The average time taken to run test 5 was %f ms\n",avg);
	
}

