#include <stdio.h>
void transpose_blocking(int n, int blocksize, int *dst, int *src);
int main() {
  int n = 100;
  int blocksize = 20;
  int *dst = (int *)malloc(sizeof(int) * n);
  int *src = (int *)malloc(sizeof(int) * n);
  transpose_blocking(n, blocksize, dst, src);
  free(dst);
  free(src);
}

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
      if (n - xStart + 1 != blocksize) {
        xBound = n - xStart + 1;
      }
      for (int x = xStart; x < xBound; x++) {
        if (n - yStart + 1 != blocksize) {
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