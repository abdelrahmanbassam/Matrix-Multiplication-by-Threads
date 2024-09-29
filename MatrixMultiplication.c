#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
// #include <string.h>
// #include <math.h>


struct Matrix a;
struct Matrix b;
int **result;
struct Matrix 
{
   int rows;
   int cols;
   int **matrix;
};
struct timeval stop, start;

struct threadParams{
  int row;
  int col;
};

void printMatrix(int **m,int row,int cols){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cols; j++){
            printf("%d ",m[i][j]);
        }
        printf("\n");
    }
}

struct Matrix scanMatrix(char *filePath){
    FILE *file = fopen(filePath, "r");
    struct Matrix m;
    fscanf(file, "row=%d col=%d", &m.rows, &m.cols);
    m.matrix = (int**)malloc(m.rows * sizeof(int *));
    for(int i = 0; i < m.rows; i++){
        m.matrix[i] = (int *)malloc(m.cols * sizeof(int));
        for(int j = 0; j < m.cols; j++){
            fscanf(file, "%d", &m.matrix[i][j]);
        }
    }
    fclose(file);
    return m;
}
void oneMatrixMulti(){
  for(int i = 0; i < a.rows; i++){
    for(int j = 0; j < b.cols; j++){
      for(int k = 0; k < a.cols; k++){
        result[i][j] += a.matrix[i][k] * b.matrix[k][j];
      }
    }
  }
 
}
void oneThreadMulti(){
  pthread_t thread;
  pthread_create(&thread, NULL, oneMatrixMulti, NULL);
  pthread_join(thread, NULL);
}

void* rowMatrixMulti(void* params){
  int row = (intptr_t)params;
  for(int i = 0; i < b.cols; i++){
    for(int j = 0; j < a.cols; j++){
      result[row][i] += a.matrix[row][j] * b.matrix[j][i];
    }
  }
}

void rowThreadMulti(){
  pthread_t *threads = malloc(a.rows * sizeof(pthread_t));

  for(int i = 0; i < a.rows; i++){
    pthread_create(&threads[i], NULL, rowMatrixMulti, (void *)(intptr_t)i);
  }

  for(int i = 0; i < a.rows; i++){
    pthread_join(threads[i], NULL);
  }

  free(threads);
}

void* elementMatrixMulti(void* params){
  struct threadParams *info = (struct threadParams *)params;
  int row = info->row;
  int col = info->col;
  for(int i=0; i<a.cols; i++){
    result[row][col] += a.matrix[row][i] * b.matrix[i][col];
  }

}

void elementTheadMulti(){
  int totalThreads = a.rows * b.cols;
  pthread_t *threads = malloc(totalThreads * sizeof(pthread_t));
  struct threadParams *paramsArray = malloc(totalThreads * sizeof(struct threadParams));

  for(int i = 0; i < a.rows; i++){
    for(int j = 0; j < b.cols; j++){
      int index = i * b.cols + j;
      paramsArray[index].row = i;
      paramsArray[index].col = j;
      pthread_create(&threads[index], NULL, elementMatrixMulti, &paramsArray[index]);
    }
  }

  for(int i = 0; i < totalThreads; i++){
    pthread_join(threads[i], NULL);
  }

  free(threads);
  free(paramsArray);
}

int **alocateMatrix(int rows, int cols){
    int **m = (int**)malloc(rows * sizeof(int *));
 for(int i = 0; i < a.rows; i++){
    m[i] = malloc(b.cols * sizeof(int));
    for(int j = 0; j < b.cols; j++){
        m[i][j] = 0;
    }
}
    return m;
}

void writeResultToFile(char *filePath, char *method, int **m){
    FILE *file = fopen(filePath, "w");
    fprintf(file, "Method: %s\n", method);
    fprintf(file, "row=%d col=%d\n", a.rows, b.cols);
    for(int i = 0; i < a.rows; i++){
        for(int j = 0; j < b.cols; j++){
            fprintf(file, "%d ", m[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}


int main(){
   
    a = scanMatrix("test1/a.txt");
    b = scanMatrix("test1/b.txt");
    //////////////////////////////
    result=alocateMatrix(a.rows, b.cols);
    gettimeofday(&start, NULL); 

    oneThreadMulti();

    gettimeofday(&stop, NULL); 
    printf("Microseconds taken in A thread per matrix::  %lu\n", stop.tv_usec - start.tv_usec);
    printf("number of threads: 1\n\n");
    writeResultToFile("test1/resultOneThread.txt", "oneThreadMulti", result);
    //////////////////////////////
    result=alocateMatrix(a.rows, b.cols);
    gettimeofday(&start, NULL); 
    
    rowThreadMulti();

    gettimeofday(&stop, NULL); 
    printf("Microseconds taken in A thread per row::  %lu\n", stop.tv_usec - start.tv_usec);
    printf("number of threads: %d\n\n", a.rows);
    writeResultToFile("test1/resultRowThread.txt", "rowThreadMulti", result);
    //////////////////////////////
    
    result=alocateMatrix(a.rows, b.cols); 
    gettimeofday(&start, NULL); 

    elementTheadMulti();

    gettimeofday(&stop, NULL); 
    printf("Microseconds taken in A thread per element::  %lu\n", stop.tv_usec - start.tv_usec);
    printf("number of threads: %d\n\n", a.rows * b.cols);
    writeResultToFile("test1/resultElementThead.txt", "elementTheadMulti", result);

    return 0;
}