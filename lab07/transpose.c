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
  int xRepBound = n / blocksize;
  int yRepBound = n / blocksize;
  // if n==5, blocksize=2, we should repeat 3 times instead of 2
  // FYI read line 32
  if (n % blocksize != 0) {
    xRepBound += 1;
    yRepBound += 1;
  }
  int xStart = 0;
  int yStart = 0;
  int xBound = blocksize;
  int yBound = blocksize;

  for (int xRep = 0; xRep < xRepBound; xRep++) {
    for (int yRep = 0; yRep < yRepBound; yRep++) {
      /* if n is not multiple of blocksize
       * we need to deal with the rest.
       * let's say n==11 blocksize==3
       * when xStart==9, the xBound should be 2 instead of
       * 3(blocksize)
       */
      if (n - xStart + 1 == blocksize) {
        xBound = n - xStart + 1;
      }
      for (int x = xStart; x < xBound; x++) {
        if (n - yStart + 1 == blocksize) {
          yBound = n - yStart + 1;
        }
        for (int y = yStart; y < yBound; y++) {
          dst[y + x * n] = src[x + y * n];
        }
        yStart += blocksize;
      }
      xStart += blocksize;
    }
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
