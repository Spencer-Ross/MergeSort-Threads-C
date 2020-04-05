/***********************************************************************
name: Spencer Ross
    assignment7 -- Multi-Threaded Quick Sort
description:    Uses specified number of threads to sort a list of words
                using quick sort. Upon partitioning, the data is either
                sent to a thread if the number of total threads hasn't
                been reached, or the the sort function recurses. Once
                each thread has finished, the threads join the data
                back together.

***********************************************************************/

/* Includes and definitions */
#include "assignment7.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define SORT_THRESHOLD    40

typedef struct _sortParams {
    char** array;
    int left;
    int right;
} SortParams;

static int maximumThreads;      /* maximum # of threads to be used */
static int activeThreads;       //# of threads created and active
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* threadFunc(void *p);

/* This is an implementation of insert sort, which although it is */
/* n-squared, is faster at sorting short lists than quick sort,   */
/* due to its lack of recursive procedure call overhead.          */
static void insertSort(char** array, int left, int right) {
    int i, j;
    for (i = left + 1; i <= right; i++) {
        char* pivot = array[i];
        j = i - 1;
        while (j >= left && (strcmp(array[j],pivot) > 0)) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = pivot;
    }
}


/* Recursive quick sort, but with a provision to use */
/* insert sort when the range gets small.       */
static void quickSort(void* p) {
    SortParams* params = (SortParams*) p;
    int subFlag = 0;
    pthread_t subT;
    char** array = params->array;
    int left = params->left;
    int right = params->right;
    int i = left, j = right;
    
    if (j - i > SORT_THRESHOLD) {           /* if the sort range is substantial, use quick sort */

        int m = (i + j) >> 1;               /* pick pivot as median of         */
        char* temp, *pivot;                 /* first, last and middle elements */
        if (strcmp(array[i],array[m]) > 0) {
            temp = array[i]; array[i] = array[m]; array[m] = temp;
        }
        if (strcmp(array[m],array[j]) > 0) {
            temp = array[m]; array[m] = array[j]; array[j] = temp;
            if (strcmp(array[i],array[m]) > 0) {
                temp = array[i]; array[i] = array[m]; array[m] = temp;
            }
        }
        pivot = array[m];

        for (;;) {
            while (strcmp(array[i],pivot) < 0) i++; /* move i down to first element greater than or equal to pivot */
            while (strcmp(array[j],pivot) > 0) j--; /* move j up to first element less than or equal to pivot      */
            if (i < j) {
                char* temp = array[i];      /* if i and j have not passed each other */
                array[i++] = array[j];      /* swap their respective elements and    */
                array[j--] = temp;          /* advance both i and j                  */
            } else if (i == j) {
                i++; j--;
            } else break;                   /* if i > j, this partitioning is done  */
        }
        
        SortParams first;  first.array = array; first.left = left; first.right = j;
        pthread_mutex_lock(&mutex);
        if(activeThreads < maximumThreads) {
            activeThreads++;
            pthread_mutex_unlock(&mutex);
            pthread_create(&subT, NULL, &threadFunc, &first);
            subFlag = 1;
        } else {
            pthread_mutex_unlock(&mutex);
            quickSort(&first);      /* sort the left partition  */
        }
        
        SortParams second; second.array = array; second.left = i; second.right = right;
        quickSort(&second);     /* sort the right partition */
    } else insertSort(array,i,j);       /* for a small range use insert sort */
    if(subFlag) {
        pthread_join(subT, NULL);
        activeThreads--;
    }
}


/** user interface routine to set the number 
    of threads sortT is permitted to use */
void setSortThreads(int count) {
    maximumThreads = count;
    activeThreads = 0;
}

//thread function to make sure threads are on same page
void* threadFunc(void *p) {
    quickSort(p);
    return NULL;
}


/** user callable sort procedure, 
    sorts array of count strings, 
    beginning at address array */
void sortThreaded(char** array, unsigned int count) {
    SortParams parameters;
    parameters.array = array; parameters.left = 0; parameters.right = count - 1;
    quickSort(&parameters);
}
