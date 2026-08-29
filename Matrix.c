#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


//Matrix allocation 
double *alloc_matrix(int N) {
    double *M = malloc(N * N * sizeof(double));
    if (!M) {
        fprintf(stderr, "Error: could not allocate a %dx%d matrix\n", N, N);
        exit(1);
    }
    return M;
}

//Fill matrix with 0, and value along the diagonal
void fill_matrix(double *M, int N, double val) {
    memset(M, 0, (size_t)N * (size_t)N * sizeof(double));
    for (int i = 0; i < N; i++) M[i*N + i] = val;
}

//Check all elements if they are below a threshold
int check_all(const double *C, int N, double a, double b, double thr) {
    double expected_diag = a * b;
    double expected_val = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (i == j) expected_val = expected_diag;
            else expected_val = 0.0;
            
            if (fabs(C[i*N + j] - expected_val) > thr) return 0;
        }
    return 1;
}

//Check only the diagonal since A and B are diagonal matrices 
int check_diag(const double *C, int N, double a, double b, double thr) {
    double expected_diag = a * b;
    for (int i = 0; i < N; i++)
        if (fabs(C[i*N + i] - expected_diag) > thr) return 0;
    return 1;
}

void save_matrix(const char *filename, const double *C, int N) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error: cannot open '%s' for writing\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%d %d\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            fprintf(f, "%.6f ", C[i * N + j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char **argv) {

    //Input parameters
    if (argc != 5) {
       fprintf(stderr, "Usage: %s N a b fileout\n", argv[0]);
       fprintf(stderr, "  A = a*I_N, B = b*I_N, C = A*B, saved to 'fileout'\n");
       return 1;
    }

    int N = atoi(argv[1]);
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    const char *fileout = argv[4];

    if (N <= 0) {
       fprintf(stderr, "Error: N must be a positive integer\n");
       return 1;
    }

    //Allocation
    double *A = alloc_matrix(N);
    double *B = alloc_matrix(N);
    double *C = alloc_matrix(N);
    double *C_opt = alloc_matrix(N);

    //Filling of A and B
    fill_matrix(A, N, a);
    fill_matrix(B, N, b);

    //Start clock
    clock_t start = clock();

    //Multiply in order ijk
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                C[i*N + j] += A[i*N + k] * B[k*N + j];
            }
        }
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nNaive method time: %f ms\n", elapsed*1000.0);

    start = clock();

    //Optimized multiplication in order ikj
    for(int i = 0; i < N; i++){
        for(int k = 0; k < N; k++){
            for(int j = 0; j < N; j++){
                C_opt[N*i + j] += A[i*N + k] * B[k*N + j];
            }
        }
    }

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Optimized method time: %f ms\n", elapsed*1000.0);

    start = clock();
    int full_ok = check_all(C_opt, N, a, b, 1e-9);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    if(full_ok) printf("Check all matrix time: %f ms\n", elapsed*1000.0);
    else printf("FAIL CHECK\n");
 
    start = clock();
    int fast_ok = check_diag(C_opt, N, a, b, 1e-9);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    if(fast_ok) printf("Check diagonal time: %f ms\n", elapsed*1000.0);
    else printf("FAIL CHECK\n");
    
    save_matrix(fileout, C_opt, N);
    printf("\nMatrix C saved to '%s'\n", fileout);


    free(A);
    free(B);
    free(C);
    free(C_opt);

    return 0;
}
