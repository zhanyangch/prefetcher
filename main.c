#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#define TEST_W 4096
#define TEST_H 4096

/* provide the implementations of naive_transpose,
 * sse_transpose, sse_prefetch_transpose
 */

#include "impl.c"
void (*transpose)(int *src, int *dst, int w, int h);

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main()
{
#ifdef NAIVE
    transpose=&naive_transpose;
#endif

#ifdef SSE
    transpose=&sse_transpose;
#endif

#ifdef SSE_PREFETCH
    transpose=&sse_prefetch_transpose;
#endif

    struct timespec start, end;
    int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
    int *out0 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
    int *out1 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
    int *out2 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

    srand(time(NULL));
    for (int y = 0; y < TEST_H; y++)
        for (int x = 0; x < TEST_W; x++)
            *(src + y * TEST_W + x) = rand();

    clock_gettime(CLOCK_REALTIME, &start);
    transpose(src, out0, TEST_W, TEST_H);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("time: \t %ld us\n", diff_in_us(start, end));

    free(src);
    free(out0);
    free(out1);
    free(out2);

    return 0;
}
