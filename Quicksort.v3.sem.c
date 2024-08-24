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
void *quickSort_thread(void *arg) {
    quicksort_args *args = (quicksort_args*) arg;
    quickSort(args->arr, args->low, args->high);
    sem_post(&semaphore); // Libera o semáforo ao terminar
    pthread_exit(NULL);
}

void printArray(int arr[], int size){
   int i;
   for (i=0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main(){

   long int n = 100000000;
   int *arr = (int*)malloc(n * sizeof(int));
   srand(0);
   long int i;
   for (i = 0; i < n; i++) {
      arr[i] = rand() % n;
   }
   
   sem_init(&semaphore, 0, 4);  // Inicializa o semáforo com o valor máximo de threads
   
   clock_t start, end;
   double cpu_time_used;
     
   start = clock();
   quicksort_args args = {arr, 0, n - 1};
   end = clock();
  
   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
   
   printf("CLOCKS_PER_SEC: %ld \n", CLOCKS_PER_SEC);
   printf("CPU time used is: %f \n", cpu_time_used);
   
   
    // Cria a thread principal do quicksort
    pthread_t main_thread;
    sem_wait(&semaphore);  // Espera pelo semáforo antes de criar a thread
    pthread_create(&main_thread, NULL, quickSort_thread, &args);

    pthread_join(main_thread, NULL);  // Aguarda a thread principal terminar

    printf("The sorted array is: \n");
    printArray(arr, n);

    sem_destroy(&semaphore);  // Destrói o semáforo
    free(arr);
   
   
   return 0;
}