// Use compiled program enetring the variables for d=ax+y:
// ./Vec_sum N a x y
	
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s N a x y\n", argv[0]);
        fprintf(stderr, "  N : vector dimension (positive integer)\n");
        fprintf(stderr, "  a : scalar (double)\n");
        fprintf(stderr, "  x : uniform vector of elements x (double)\n");
        fprintf(stderr, "  y : uniform vector of elements y (double)\n");
        return 1;
    }

// Allocate variables
    long long N = atoll(argv[1]);
    double a = atof(argv[2]);
    double x_val = atof(argv[3]);
    double y_val = atof(argv[4]);

    if (N <= 0) {
        fprintf(stderr, "Error: N must be a positive integer\n");
        return 1;
    }

// Dynamic allocation of memory because of potentially large N
    double *x = (double *)malloc((size_t)N * sizeof(double));
    double *y = (double *)malloc((size_t)N * sizeof(double));
    double *d = (double *)malloc((size_t)N * sizeof(double));

// Fill vectors
    for (long long i = 0; i < N; i++) {
        x[i] = x_val;
        y[i] = y_val;
    }

// Compute vector sum
    for (long long i = 0; i < N; i++) {
        d[i] = a * x[i] + y[i];
    }

// Check correctness of each element
    double d_exp = a * x_val + y_val;
    long long errors = 0;
    
    for (long long i = 0; i < N; i++) {
        if (d[i] != d_exp) {
            errors++;
        }
    }

// Print
    printf("Vector length: %lld\n", N);
    printf("a: %.17g\n", a);
    printf("x: %.17g\n", x_val);
    printf("y: %.17g\n", y_val);
    printf("ax+y: %.17g\n", d_exp);
    printf("d[0]: %.17g\n", d[0]);
    printf("Number of incorrect elements of d[i]: %lld\n", errors);

    free(x);
    free(y);
    free(d);

    return 0;
}
