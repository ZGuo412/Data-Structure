
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "string.h"



#define T_SIZE 1000000



void sort(int list[], int n);


int compare(int array1[], int array2[], int n)

{
  int i;
  for (i=0; i<n; i++)
    if (array1[i] != array2[i])
    {
      /* THIS BLOCK ADDED FOR READABILITY */
      printf("\nError in element %d/%d\n",i+1,n);
      printf("Correct values is %d\n",array2[i]);
      printf("Returned values is %d\n",array1[i]);
      /* END BLOCK */
      return 0;
    }
  return 1;
}

void sol_swap(int a[], int i, int j)
{
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void sol_quicksort(int a[], int left, int right)
{
    int last = left, i;

    if (left >= right) return;

    sol_swap(a,left,left + rand() % (right-left+1));
    for (i = left + 1; i <= right; i++)
        if (a[i] < a[left])
            sol_swap(a,++last,i);
    sol_swap(a,left,last);
    sol_quicksort(a,left,last-1);
    sol_quicksort(a,last+1,right);
}

void sort_sol(int array[], int n)
{
  sol_quicksort(array,0,n-1);
}

int main(int arcg, char *argv[])
{
  int n, i;
  int *array1;
  int *array2;
  clock_t start, end;

  sscanf(argv[1],"%d",&n);
  array1 = (int*)malloc(T_SIZE*sizeof(int));
  array2 = (int*)malloc(T_SIZE*sizeof(int));

  for (i=0; i<n; i++)
  {
    array1[i] = i-n/2;
    array2[i] = array1[i];
  }
  start = clock();
  sort(array1,n);
  end = clock();
  if (compare(array1,array2,n))
    printf("%.4f\t", ((double) (end - start)) / CLOCKS_PER_SEC);
  else
    printf("BAD\t");

  for (i=0; i<n; i++)
  {
    array1[i] = n/2 - i;
    array2[i] = array1[i];
  }
  start = clock();
  sort(array1,n);
  end = clock();
  sort_sol(array2,n);
  if (compare(array1,array2,n))
    printf("%.4f\t", ((double) (end - start)) / CLOCKS_PER_SEC);
  else
    printf("BAD\t");

  srand(44231);
  for (i=0; i<n; i++)
  {
    if (i>n/10)
      array1[i] = rand() - RAND_MAX/2;
    else
      array1[i] = 0;
    array2[i] = array1[i];
  }
  start = clock();
  sort(array1,n);
  end = clock();
  sort_sol(array2,n);
  if (compare(array1,array2,n))
    printf("%.4f\t", ((double) (end - start)) / CLOCKS_PER_SEC);
  else
    printf("BAD\t");

  srand(1313);
  for (i=0; i<n; i++)
  {
    array1[i] = rand() - RAND_MAX/2;
    array2[i] = array1[i];
  }
  start = clock();
  sort(array1,n);
  end = clock();
  sort_sol(array2,n);
  if (compare(array1,array2,n))
    printf("%.4f\t", ((double) (end - start)) / CLOCKS_PER_SEC);
  else
    printf("BAD\t");

  srand(12345);
  for (i=0; i<n; i++)
  {
    array1[i] = rand() - RAND_MAX/2;
    array2[i] = array1[i];
  }
  start = clock();
  sort(array1,n);
  end = clock();
  sort_sol(array2,n);
  if (compare(array1,array2,n))
    printf("%.4f\t", ((double) (end - start)) / CLOCKS_PER_SEC);
  else
    printf("BAD\t");

  free(array1);
  free(array2);
  return 0;
}
