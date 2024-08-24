#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

typedef struct {
    int *arr;
    long int low;
    long int high;
} quicksort_args;

void swap(int* a, int* b) {
   int t = *a;
   *a = *b;
   *b = t;
}

int partition (int arr[], int low, int high) {
   int pivot = arr[high];
   int i = (low - 1);
   for (int j = low; j <= high- 1; j++){
      if (arr[j] <= pivot){
         i++;
         swap(&arr[i], &arr[j]);
      }
   }
   swap(&arr[i + 1], &arr[high]);
   return (i + 1);
}

void quickSort(int arr[], long int low, long int high){
   if (low < high){
      long int pi = partition(arr, low, high);
      quickSort(arr, low, pi - 1);
      quickSort(arr, pi + 1, high);
   }
}

void printArray(int arr[], int size){
   int i;
   for (i=0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main(){
   long int n = 10000;
   int *arr = (int*)malloc(n * sizeof(int));
   srand(0);
   long int i;
   for (i = 0; i < n; i++) {
      arr[i] = rand() % n;
   }
   
   quickSort(arr, 0, n-1);

   printf("The sorted array is: \n");
   printArray(arr, n);
   return 0;
}