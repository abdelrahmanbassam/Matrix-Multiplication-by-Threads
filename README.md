# Untitled

# Matrix Multiplication (Multi-Threading)

## Overview

This project implements a multi-threaded matrix multiplication program using the **Pthread** library. The program performs matrix multiplication in three different ways to compare performance in terms of thread creation and execution time. The input consists of two matrices read from text files, and the output matrix is written to text files corresponding to each method.

Matrix multiplication is parallelized using the following three methods:

1. **A thread per matrix**: One thread handles the entire multiplication.
2. **A thread per row**: Each thread computes a single row of the output matrix.
3. **A thread per element**: Each thread computes a single element of the output matrix.

## System Requirements

- **Operating System**: Linux
- **Programming Language**: C
- **Libraries**: Pthread
- **Input Format**: Text files for matrices

## Compilation and Execution

To compile and run the matrix multiplication program:

### Compilation:

```bash
gcc -pthread -o matMultp MatrixMultiplication.c
```

### Execution:

```bash
./matMultp Mat1 Mat2 MatOut
```

- **Mat1**: Input file for matrix A.
- **Mat2**: Input file for matrix B.
- **MatOut**: Output file prefix for matrix C.

If no arguments are provided, the default input files are `a.txt` for matrix A, `b.txt` for matrix B, and `c` as the output file prefix.

### Example:

```bash
./matMultp a b c
```

- Input files: `a.txt` and `b.txt`.
- Output files: `c_per_matrix.txt`, `c_per_row.txt`, and `c_per_element.txt`.

## Input Format

The input files should be in the following format:

```bash
row=x col=y
1 2 3 4 5
6 7 8 9 10
11 12 13 14 15
```

Where `x` is the number of rows, `y` is the number of columns, and the subsequent lines represent the matrix elements.

## Output Format

The program outputs three text files representing the results of the matrix multiplication for each method:

1. **c_per_matrix.txt**
2. **c_per_row.txt**
3. **c_per_element.txt**

Each output file follows this format:

```bash
Method: [Description of the method]
row=x col=z
[Resulting matrix]
```

Example:

```bash
Method: A thread per row
row=2 col=2
1 2
3 4
```

## Matrix Multiplication Methods

### 1. A Thread Per Matrix

- In this method, a single thread performs the entire matrix multiplication.
- This is the simplest form of threading, where minimal overhead is introduced because only one thread is created.
- **Output file**: `c_per_matrix.txt`

### 2. A Thread Per Row

- Each thread computes one row of the resulting matrix. The number of threads created equals the number of rows in matrix A.
- This method achieves parallelism by dividing work across multiple threads but introduces some overhead due to thread creation.
- **Output file**: `c_per_row.txt`

### 3. A Thread Per Element

- Each thread computes one element of the resulting matrix. The number of threads created equals the number of elements in matrix C (i.e., rows of matrix A times columns of matrix B).
- This method offers the highest degree of parallelism but also introduces the most overhead due to a large number of threads being created.
- **Output file**: `c_per_element.txt`

## Execution Time and Thread Count

For each method, the program prints the following statistics to the console:

1. The number of threads created.
2. The time taken for matrix multiplication using that method.

### Time Measurement

The program uses the following code to measure execution time:

```bash
#include <sys/time.h>struct timeval stop, start;
gettimeofday(&start, NULL)
gettimeofday(&stop, NULL);

printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
```

## Error Handling

The program handles errors gracefully by checking for:

- Incorrect matrix dimensions (e.g., if the number of columns in matrix A doesn't match the number of rows in matrix B).
- File reading or writing errors.
- Incorrect input formatting.

If an error occurs, the program will display an appropriate error message and terminate.

## Memory Management

The program ensures proper memory management by:

- Dynamically allocating memory for matrices A, B, and C.
- For the "A thread per row" method, passing only the row number or a structure containing pointers to the matrices and row numbers.
- For the "A thread per element" method, passing either the row and column numbers or a structure containing pointers to the matrices and the respective row and column.
- Freeing all dynamically allocated memory before thread termination to avoid memory leaks.

## Method Comparison

### 1. A Thread Per Matrix

- **Number of Threads**: 1
- **Execution Time**: Fastest (least overhead)

### 2. A Thread Per Row

- **Number of Threads**: Equal to the number of rows in matrix A.
- **Execution Time**: Slower than method 1, but suitable for larger matrices.

### 3. A Thread Per Element

- **Number of Threads**: Equal to the number of elements in matrix C (rows of A * columns of B).
- **Execution Time**: Slowest due to high thread creation overhead, but provides the most parallelism.

## Sample Output

### Console Output:

```bash
Method: A thread per matrix
Threads created: 1
Seconds taken: 0
Microseconds taken: 500

Method: A thread per row
Threads created: 3
Seconds taken: 0
Microseconds taken: 1000

Method: A thread per element
Threads created: 9
Seconds taken: 0
Microseconds taken: 2000
```

### Output File: `c_per_matrix.txt`

```php
Method A thread per matrix
row=3 col=3
58 64 69
139 154 169
220 244 269
```
