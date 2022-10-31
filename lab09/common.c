#include "common.h"
#include <emmintrin.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

long long int sum(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();

  long long int sum = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  long long int sum = 0;

  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      if (vals[i] >= 128)
        sum += vals[i];
      if (vals[i + 1] >= 128)
        sum += vals[i + 1];
      if (vals[i + 2] >= 128)
        sum += vals[i + 2];
      if (vals[i + 3] >= 128)
        sum += vals[i + 3];
    }

    // This is what we call the TAIL CASE
    // For when NUM_ELEMS isn't a multiple of 4
    // NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than
    // NUM_ELEMS
    for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        sum += vals[i];
      }
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return sum;
}

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(
      127); // This is a vector with 127s in it... Why might you need this?
  long long int result = 0; // This is where you should put your final result!
  /* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */

  int tempResult[4];
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* YOUR CODE GOES HERE */
    unsigned int i;
    __m128i formerValues = _mm_setzero_si128();
    for (i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
      __m128i currentValues = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i cmp = _mm_cmpgt_epi32(currentValues, _127);
      // if vectorValues > _127, its values wouldn't be changed
      currentValues = _mm_and_si128(currentValues, cmp);
      formerValues = _mm_add_epi32(currentValues, formerValues);
    }

    /* You'll need a tail case. */
    _mm_storeu_si128((__m128i *)tempResult, formerValues);
    for (; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        tempResult[0] += vals[i];
      }
    }

    for (int j = 0; j < 4; j++) {
      result += tempResult[j];
    }
  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;

  int tempResult0[4];
  int tempResult1[4];
  int tempResult2[4];
  int tempResult3[4];
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    /* COPY AND PASTE YOUR sum_simd() HERE */
    /* MODIFY IT BY UNROLLING IT */
    unsigned int i;
    __m128i formerValues0 = _mm_setzero_si128();
    __m128i formerValues1 = _mm_setzero_si128();
    __m128i formerValues2 = _mm_setzero_si128();
    __m128i formerValues3 = _mm_setzero_si128();
    for (i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
      __m128i currentValues0 = _mm_loadu_si128((__m128i *)(vals + i));
      __m128i cmp0 = _mm_cmpgt_epi32(currentValues0, _127);
      // if vectorValues > _127, its values wouldn't be changed
      currentValues0 = _mm_and_si128(currentValues0, cmp0);
      formerValues0 = _mm_add_epi32(currentValues0, formerValues0);

      __m128i currentValues1 = _mm_loadu_si128((__m128i *)(vals + i + 4));
      __m128i cmp1 = _mm_cmpgt_epi32(currentValues1, _127);
      currentValues1 = _mm_and_si128(currentValues1, cmp1);
      formerValues1 = _mm_add_epi32(currentValues1, formerValues1);

      __m128i currentValues2 = _mm_loadu_si128((__m128i *)(vals + i + 8));
      __m128i cmp2 = _mm_cmpgt_epi32(currentValues2, _127);
      currentValues2 = _mm_and_si128(currentValues2, cmp2);
      formerValues2 = _mm_add_epi32(currentValues2, formerValues2);

      __m128i currentValues3 = _mm_loadu_si128((__m128i *)(vals + i + 12));
      __m128i cmp3 = _mm_cmpgt_epi32(currentValues3, _127);
      currentValues3 = _mm_and_si128(currentValues3, cmp3);
      formerValues3 = _mm_add_epi32(currentValues3, formerValues3);
    }

    /* You'll need 1 or maybe 2 tail cases here. */
    _mm_storeu_si128((__m128i *)tempResult0, formerValues0);
    for (; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        tempResult0[0] += vals[i];
      }
    }

    _mm_storeu_si128((__m128i *)tempResult1, formerValues1);
    _mm_storeu_si128((__m128i *)tempResult2, formerValues2);
    _mm_storeu_si128((__m128i *)tempResult3, formerValues3);
    for (int j = 0; j < 4; j++) {
      result += tempResult0[j];
    }
    for (int j = 0; j < 4; j++) {
      result += tempResult1[j];
    }
    for (int j = 0; j < 4; j++) {
      result += tempResult2[j];
    }
    for (int j = 0; j < 4; j++) {
      result += tempResult3[j];
    }

  }
  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}