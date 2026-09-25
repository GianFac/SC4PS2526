/* =====================================================================
 * HomeWork 07 - Random number generation, LLN, Monte Carlo, transforms
 * =====================================================================
 * Output files:
 *   Ex1.csv, Ex2.csv, Ex3.csv, Ex4_5.csv
 *
 * Compile:
 *   gcc -O2 -o homework07 All_tasks.c -lm
 * =====================================================================
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// LCG 
static inline uint32_t lcg32_next(uint32_t *state) {
    const uint32_t a = 1664525u;
    const uint32_t c = 1013904223u;
    *state = a * (*state) + c;
    return *state;
}

static inline double lcg32_next_double(uint32_t *state) {
    return lcg32_next(state) / 4294967296.0;
}

/* =====================================================================
 * Exercise 1 - Coin tosses and LLN
 * ===================================================================== */
static void run_ex1(void) {
    uint32_t seed = 123456789u;
    const int N = 100000;
    int heads = 0;

    FILE *f = fopen("data/Ex1.csv", "w");
    fprintf(f, "n,heads\n");

    for (int i = 1; i <= N; i++) {
        double u = lcg32_next_double(&seed);

        heads += (int)((u < 0.5) ? 1 : 0);
	
        fprintf(f, "%d,%d\n", i, heads);
        if(i%(N/10)==0){
        	printf("n toss:%d n heads:%d fraction:%.10f\n", i, heads, (double)heads / i);
        	}
    }

    fprintf(stderr, "Final fraction of heads = %.10f\n", (double)heads / N);
    fclose(f);
}

/* =====================================================================
 * Exercise 2 - Monte Carlo estimate of pi
 * ===================================================================== */
static double estimate_pi(int n, uint32_t seed_x, uint32_t seed_y) {

    int hit = 0;

    for (int i = 0; i < n; ++i) {
        double x = lcg32_next_double(&seed_x);
        double y = lcg32_next_double(&seed_y);

        if (x * x + y * y <= 1.0) {
            hit++;
        }
    }

    return 4.0 * hit / n;
}

static void run_ex2(void) {

    int samples[] = {100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    int n_samp = (int)(sizeof(samples) / sizeof(samples[0]));
    FILE *f = fopen("data/Ex2.csv", "w");
    fprintf(f, "n,pi_estimate\n");
    for (int i = 0; i < n_samp; ++i) {
        double pi_est = estimate_pi(samples[i], 13579u, 24680u);
        printf("%d %.10f\n", samples[i], pi_est);
        fprintf(f, "%d,%.10f\n", samples[i], pi_est);
    }
    fclose(f);
}

/* =====================================================================
 * Exercise 3 - Change of variables Y = U^2
 * ===================================================================== */
static void run_ex3(void) {
    uint32_t state = 987654321u;
    const int n = 1000000;
    FILE *f = fopen("data/Ex3.csv", "w");
    fprintf(f, "y\n");

    for (int i = 0; i < n; ++i) {
        double u = lcg32_next_double(&state);
        double y = u * u;
        fprintf(f, "%.12f\n", y);
    }

    fclose(f);
}

/* =====================================================================
 * Exercise 4/5 - Inverse-transform exponential + empirical CDF
 * ===================================================================== */
static void run_ex4_5(void) {
    uint32_t state = 987654321u;
    const int n = 50000;
    const double lambda = 1.5;
    double *y = malloc((size_t)n * sizeof(double));
    double *cdf = malloc((size_t)n * sizeof(double));
    FILE *f = fopen("data/Ex4_5.csv", "w");
    fprintf(f, "y,CDF_x,CDF_y\n");

    for (int i = 0; i < n; ++i) {
        double u = lcg32_next_double(&state);
        y[i] = -1 * log(1.0 - u) / lambda;
        cdf[i] = y[i];
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (cdf[j] > cdf[j + 1]) {
                double temp = cdf[j];
                cdf[j] = cdf[j + 1];
                cdf[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        fprintf(f, "%lf,%lf,%lf\n", y[i], cdf[i], (double)(i + 1) / n);
    }
    fclose(f);
    free(y);
    free(cdf);
}

/* ===================================================================== */

int main(void) {
    printf("=== Exercise 1: coin tosses and the LLN ===\n");
    run_ex1();

    printf("\n=== Exercise 2: Monte Carlo estimate of pi ===\n");
    run_ex2();

    printf("\n=== Exercise 3: Y = U^2 ===\n");
    run_ex3();

    printf("\n=== Exercise 4/5: inverse-transform exponential + empirical CDF ===\n");
    run_ex4_5();

    printf("\nDone. Wrote Ex1.csv, Ex2.csv, Ex3.csv, Ex4_5.csv\n");
    return 0;
}
