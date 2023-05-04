
#include <immintrin.h> // portable to all x86 compilers
#include <stdio.h>
#include <time.h>

#define DATA float

const int SIZE = 128;

DATA __attribute__((aligned(16))) A[SIZE][SIZE];
DATA __attribute__((aligned(16))) B[SIZE][SIZE];
DATA __attribute__((aligned(16))) Matrix_transposed[SIZE][SIZE];

double seconds()
{

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
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

void initialize_matrix_Transposed(DATA matrix[SIZE][SIZE], DATA transposedMatrix[SIZE][SIZE], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }
}

DATA **matMat(DATA s1[SIZE][SIZE], DATA s2[SIZE][SIZE], int size)
{
    DATA **tmp = static_cast<DATA **>(malloc(sizeof(DATA *) * size));

    for (int i = 0; i < size; i++)
    {
        tmp[i] = static_cast<DATA *>(malloc(sizeof(DATA) * size));
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            tmp[i][j] = 0;
            for (int k = 0; k < size; k++)
            {
                tmp[i][j] += s1[i][k] * s2[k][j];
            }
        }
    }

    return tmp;
}

DATA **matMat_sse(DATA m1[SIZE][SIZE], DATA m2[SIZE][SIZE], int size)
{

    DATA prod = 0;
    DATA **tmp = static_cast<DATA **>(std::malloc(sizeof(DATA *) * size));
    for (int i = 0; i < size; i++)
    {
        tmp[i] = static_cast<DATA *>(std::malloc(sizeof(DATA) * size));
    }
    __m128 X, Y, Z;
    for (int i = 0; i < size; i += 1)
    {
        for (int j = 0; j < size; j += 1)
        {
            tmp[i][j] = 0;
            prod = 0;
            Z[0] = Z[1] = Z[2] = Z[3] = 0;
            for (int n = 0; n < size; n += 4)
            {
                X = _mm_load_ps(&m1[i][n]);
                Y = _mm_load_ps(&m2[j][n]);
                X = _mm_mul_ps(X, Y);
                Z = _mm_add_ps(X, Z);
            }
            for (int k = 0; k < 4; k++)
            {
                prod += Z[k];
            }
            tmp[i][j] = prod;
        }
    }

    return tmp;
}

int main()
{

    double before, after;
    int itr = 300;
    initialize_matrix(A, SIZE);
    initialize_matrix(B, SIZE);
    initialize_matrix_Transposed(B, Matrix_transposed, SIZE);
    double scaler_time = 0;
    double sse_time = 0;
    // for Scaler Product

    for (int i = 0; i < itr; i++)
    {
        before = seconds();

        DATA **scaler_result = matMat(A, B, SIZE);

        after = seconds();
        scaler_time += (after - before);
    }

    double avg_scaler_time = (scaler_time / itr);

    printf("Time for scaler:%f\n", avg_scaler_time);

    // for SSE
    for (int i = 0; i < itr; i++)
    {
        before = seconds();

        DATA **sse_result = matMat_sse(A, Matrix_transposed, SIZE);

        after = seconds();
        sse_time += (after - before);
    }
    double avg_sse_time = (sse_time / itr);

    printf("Time for SSE:%f\n", avg_sse_time);

    printf("\n\nImprovement:%f \n\n", (avg_scaler_time / avg_sse_time));

    return 0;
}