
#include <vector>
#include <immintrin.h> // portable to all x86 compilers
#include <stdio.h>
#include <time.h>

#define DATA float

const int SIZE = 1024 * 1024;

DATA __attribute__((aligned(16))) A[SIZE];
DATA __attribute__((aligned(16))) B[SIZE];

double seconds()
{

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void initialize_array(DATA a[], int size)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 2;
    }
}

void initialize_matrix(DATA a[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            a[i][j] = rand() % 2;
        }
    }
}

int vec(DATA *s1, DATA *s2, int size)
{
    int i;
    DATA prod = 0;

    for (i = 0; i < size; i++)
    {
        prod += s1[i] * s2[i];
    }
    return prod;
}

int vec_sse(DATA *m1, DATA *m2, int size)
{

    DATA prod = 0;
    int i;

    __m128 X, Y, Z;

    Z[0] = Z[1] = Z[2] = Z[3] = 0;

    for (i = 0; i < size; i += 4)
    {
        X = _mm_load_ps(&m1[i]);
        Y = _mm_load_ps(&m2[i]);
        X = _mm_mul_ps(X, Y);
        Z = _mm_add_ps(X, Z);
    }

    for (i = 0; i < 4; i++)
    {
        prod += Z[i];
    }

    return prod;
}

int main()
{
    int itr = 300;
    DATA r;
    double before, after;
    double scaler_time = 0;
    double sse_time = 0;

    initialize_array(A, SIZE);
    initialize_array(B, SIZE);
    for (int i = 0; i < itr; i++)
    {
        before = seconds();

        r = vec(A, B, SIZE);

        after = seconds();
        scaler_time += (after - before);
    }
    double avg_scaler_time = (scaler_time / itr);

    printf("Scaler:\nResult:%f Time:%f\n", r, avg_scaler_time);

    for (int i = 0; i < itr; i++)
    {
        before = seconds();
        r = vec_sse(A, B, SIZE);
        after = seconds();
        sse_time += (after - before);
    }
    double avg_sse_time = (sse_time / itr);
    printf("Vector:\nResult:%f Time:%f\n", r, avg_sse_time);
    printf("\n\nImprovement:%f \n\n", (avg_scaler_time / avg_sse_time));

    return 0;
}