#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      dst[y + x * n] = src[x + y * n];
    }
  }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
  int xStart = 0;
  int yStart = 0;
  int xBound = blocksize;
  int yBound = blocksize;
  int yRepBound = n / blocksize;
  int xRepBound = n / blocksize;
  if (n % blocksize != 0) {
    yRepBound += 1;
    xRepBound += 1;
  }

  for (int yRep = 0; yRep < yRepBound; yRep++) {
    for (int xRep = 0; xRep < xRepBound; xRep++) {

      if (n % blocksize != 0) {
        if (n - xStart < blocksize) {
          xBound = xStart + n - xStart;
        }
        if (n - yStart < blocksize) {
          yBound = yStart + n - yStart;
        }
      }
      for (int y = yStart; y < yBound; y++) {
        for (int x = xStart; x < xBound; x++) {
          dst[y + x * n] = src[x + y * n];
        }
      }
      // after finish dealing with an area, move to the right
      xStart += blocksize;
      xBound += blocksize;
    }
    // we have finished dealing with a row, now move to the next row
    yStart += blocksize;
    yBound += blocksize;
    // set the starter point to the leftmost position
    xStart = 0;
    xBound = blocksize;
  }
}

void benchmark(int *A, int *B, int n, int blocksize,
               void (*transpose)(int, int, int *, int *), char *description) {

  int i, j;
  printf("Testing %s: ", description);

  /* initialize A,B to random integers */
  srand48(time(NULL));
  for (i = 0; i < n * n; i++)
    A[i] = lrand48();
  for (i = 0; i < n * n; i++)
    B[i] = lrand48();

  /* measure performance */
  struct timeval start, end;

  gettimeofday(&start, NULL);
  transpose(n, blocksize, B, A);
  gettimeofday(&end, NULL);

  double seconds =
      (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
  printf("%g milliseconds\n", seconds * 1e3);

  /* check correctness */
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (B[j + i * n] != A[i + j * n]) {
        printf("Error!!!! Transpose does not result in correct answer!!\n");
        exit(-1);
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: transpose <n> <blocksize>\nExiting.\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  int blocksize = atoi(argv[2]);

  /* allocate an n*n block of integers for the matrices */
  int *A = (int *)malloc(n * n * sizeof(int));
  int *B = (int *)malloc(n * n * sizeof(int));

  /* run tests */
  benchmark(A, B, n, blocksize, transpose_naive, "naive transpose");
  benchmark(A, B, n, blocksize, transpose_blocking, "transpose with blocking");

  /* release resources */
  free(A);
  free(B);
  return 0;
}
