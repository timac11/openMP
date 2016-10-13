#include<stdio.h>
#include"omp.h"
#include<stdlib.h>
int ThreadNum; // getting from command line
int MassSize;
int ThreadID;
double startTime;
double endTime;
#pragma omp threadprivate (ThreadID)
void InitializeSections (){
    startTime = omp_get_wtime();
    #pragma omp parallel num_threads(1)
    {
        ThreadID = omp_get_thread_num();
    #pragma omp single
        {
        ThreadNum = omp_get_num_threads();
        printf("%d\n", ThreadNum);
        }
    }
}
int compare(const void * x1, const void * x2){
  return ( *(double*)x1 - *(double*)x2 );
}

void LocalSort (double* pData, int begin, int end){
  //double * timeArray = new double [end - begin +1];
  double* timeArray = NULL;
  timeArray = (double*) malloc((end - begin +1)*sizeof(double));
  int j;
  for (j=0; j< end - begin + 1; j++)
       timeArray[j] = pData[begin + j];
  qsort(timeArray,end-begin+1,sizeof(double),compare);
  for (j=0; j< end - begin + 1; j++)
       pData[begin + j] = timeArray[j];
  free(timeArray);
}



// Function for merging of two sorted blocks
void MergeBlocks (double* pData, int Index1, int BlockSize1, int Index2, int BlockSize2) {
  double* pTempArray = NULL;
  pTempArray = (double*) malloc((BlockSize1 + BlockSize2)*sizeof(double));
  int i1 = Index1, i2 = Index2, curr=0;
  while ((i1<Index1+BlockSize1) && (i2<Index2+BlockSize2)) {
        if (pData[i1] < pData[i2])
            pTempArray[curr++] = pData[i1++];
        else
            pTempArray[curr++] = pData[i2++];
  }
  while (i1<Index1+BlockSize1)
        pTempArray[curr++] = pData[i1++];
  while (i2<Index2+BlockSize2)
        pTempArray[curr++] = pData[i2++];
  for (int i=0; i<BlockSize1+BlockSize2; i++)
        pData[Index1+i] = pTempArray[i];
  free(pTempArray);
}

int IsSorted (double* pData, int Size) {
  int res = 0;
  for (int i=1; (i<Size)&&(res); i++) {
    if (pData[i]<pData[i-1])
      res=1;
  }
  return res;
}


void ParallelSort (double* pData, int Size){

  //#pragma omp private (ThreadID)
  InitializeSections();
  int* Index =  (int*) malloc(ThreadNum*2);
  int* BlockSize = (int*) malloc(ThreadNum*2);
  for (int i=0; i<2*ThreadNum; i++) {
    Index[i] = (int)((i*Size)/(double)(2*ThreadNum));
    if (i<2*ThreadNum-1)
        BlockSize[i] = (int) (((i+1)*Size)/(double)(2*ThreadNum) - Index[i]);
    else
        BlockSize[i] = Size - Index[i];
  }
  #pragma omp parallel
  {
    LocalSort(pData, Index[2*ThreadID], Index[2*ThreadID]+ BlockSize[2*ThreadID] - 1);
    LocalSort(pData, Index[2*ThreadID+1], Index[2*ThreadID+1]+ BlockSize[2*ThreadID+1] - 1);
  }
   int Iter = 0;
   do {
   #pragma omp parallel
        {
         if (Iter%2 == 0) { // Even iteration
            MergeBlocks(pData, Index [2*ThreadID], BlockSize[2*ThreadID],
            Index[2*ThreadID+1], BlockSize[2*ThreadID+1]);
          }
        else { // Odd iteration
            if (ThreadID<ThreadNum-1)
            MergeBlocks(pData, Index[2*ThreadID+1], BlockSize[2*ThreadID+1],
            Index[2*ThreadID+2], BlockSize[2*ThreadID+2]);
          }
        }
    Iter++;
        } while (IsSorted(pData, Size)==1);

}


// function fo comparison of two value for qsort

/*int comp(int a, int b){
 if (a > b) return 1;
 if (a == b) return 0;
 if (a < b) return -1;
}*/


int main(int argc, char ** argv){


  //ThreadNum = atoi(argv[1]);
  int MassSize;
  MassSize = 50000000;//atoi(argv[1]);
  //randomize();
  double* mass = NULL;
  mass = (double*) malloc((MassSize)*sizeof(double));
  //double mass[MassSize];
  int i;
      for ( i=0; i < MassSize; i++ ){
          //if(i%2 == 0)
          mass[i] = rand();
         //mass[i] = i;
         //printf("%g ",mass[i]);

      }
   printf("\n");
   ParallelSort(mass, MassSize);
   endTime = omp_get_wtime();

   /* for ( i=0; i < MassSize; i++ ){
          printf("%g ",mass[i]);
      }*/
    free(mass);
    printf ("%g ", endTime - startTime);
    return 0;

}



