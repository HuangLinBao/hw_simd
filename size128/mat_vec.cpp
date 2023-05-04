
#include <immintrin.h> // portable to all x86 compilers
#include <stdio.h>
#include <time.h>

#define DATA float

const int SIZE = 128;

DATA __attribute__((aligned(16))) A[SIZE];
DATA __attribute__((aligned(16))) B[SIZE][SIZE];

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

DATA *matVec(DATA s1[SIZE], DATA s2[SIZE][SIZE], int size)
{
    int i;
    float *tmp = static_cast<DATA *>(std::malloc(sizeof(DATA) * SIZE));
    // float tmp[SIZE];
    DATA prod = 0;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            tmp[i] += s2[i][j] * s1[j];
    return tmp;
}

DATA *matMat_sse(DATA m1[SIZE], DATA m2[SIZE][SIZE], int size)
{

    DATA prod = 0;
    float *tmp = static_cast<DATA *>(std::malloc(sizeof(DATA) * SIZE));
    // float tmp[SIZE];
    __m128 X, Y, Z;

    for (int i = 0; i < size; i++)
    {
        tmp[i] = 0;
        prod = 0;
        Z[0] = Z[1] = Z[2] = Z[3] = 0;
        for (int j = 0; j < size; j += 4)
        {
            X = _mm_load_ps(&m1[j]);
            Y = _mm_load_ps(&m2[i][j]);
            X = _mm_mul_ps(Y, X);
            Z = _mm_add_ps(X, Z);
        }

        for (int k = 0; k < 4; k++)
        {
            prod += Z[k];
        }
        tmp[i] = prod;
    }

    return tmp;
}

int main()
{

    double before, after;
    int itr = 300;
    initialize_array(A, SIZE);
    initialize_matrix(B, SIZE);
    double scaler_time = 0;
    double sse_time = 0;
    // for Scaler Product

    for (int i = 0; i < itr; i++)
    {
        before = seconds();

        DATA *scaler_result = matVec(A, B, SIZE);

        after = seconds();
        scaler_time += (after - before);
    }

    double avg_scaler_time = (scaler_time / itr);

    printf("Time for scaler:%f\n", avg_scaler_time);

    // for SSE
    for (int i = 0; i < itr; i++)
    {
        before = seconds();

        DATA *sse_result = matMat_sse(A, B, SIZE);

        after = seconds();
        sse_time += (after - before);
    }
    double avg_sse_time = (sse_time / itr);

    printf("Time for SSE:%f\n", avg_sse_time);

    printf("\n\nImprovement:%f \n\n", (avg_scaler_time / avg_sse_time));

    return 0;
}