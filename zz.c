#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void heapify(int A[], int heapSize, int i)
{
  int l = (2*i), r = (2*i)+1, largest;
  if (l<heapSize && A[l]>A[i])
    largest = l;
  else
    largest = i;
  if (r< heapSize && A[r]>A[largest])
    largest = r;
  if (largest != i)
    {
      int temp = A[i];
      A[i] = A[largest];
      A[largest] = temp;
      heapify(A,heapSize,largest);
    }
}
void buildHeap(int A[], int heapSize)
{
  for (int i = (heapSize/2)-1; i >= 0; i--)
    heapify(A, heapSize, i);
}
void sortHeap(int A[], int heapSize)
{
  int temp;
  buildHeap(A, heapSize);
  for (int i = heapSize-1; i >= 0; i--)
  {
    temp = A[i];
    A[i] = A[0];
    A[0] = temp;
    heapSize--;
    heapify(A,heapSize,0);
  }
}
int main()
{
  int heapSize = 0, i = 0, num = 0;
  char ch;

  FILE *fp;
  fp = fopen ("dane.txt","r");

  while(!feof(fp))
  {
    ch = fgetc(fp);
    if(ch == '\n')
    {
      heapSize++;
    }
  }

  int A[heapSize];

  rewind(fp);

  while(!feof(fp))
  {
          fscanf(fp,"%d",&A[i]);
          i++;
  }

  fclose(fp);
  fp = fopen ("result.txt","w");

  sortHeap(A, heapSize);
  
  for(int i = 0; i < heapSize; i++)
  {
    fprintf(fp,"%d \n",A[i]);
  }
}
