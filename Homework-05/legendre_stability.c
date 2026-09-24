/* =====================================================================
 * HomeWork 05 - Stability of Legendre Polynomials
 * =====================================================================
 * Compares, for m = 0 and l = 0..l_max, three ways of computing P_l(x):
 *
 *   1) forward   : standard double-precision Bonnet three-term recurrence
 *   2) reference : the same recurrence run in `long double` (extended
 *                  precision), used as a stand-in "exact" value
 *   3) backward  : Miller-style backward recurrence, started arbitrarily
 *                  at Q[L+1]=0, Q[L]=1, propagated down to l=0 and
 *                  rescaled so that Q[0] = 1 (i.e. P_0 = 1)
 *
 * Compile:
 *   gcc -std=c11 -Wall -Wextra -O2 legendre_stability.c -lm -o legendre_stability
 * Run:
 *   ./legendre_stability
 * =====================================================================
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// standard double-precision forward recurrence
void legendre_forward(double x, int lmax, double *P)
{
    P[0] = 1.0;
    if (lmax == 0) {
        return;
    }

    P[1] = x;
    for (int l = 1; l < lmax; ++l) {
        P[l + 1] = ((2.0 * l + 1.0) * x * P[l]
                    - (double)l * P[l - 1]) / (double)(l + 1);
    }
}

// high-precision reference: same recurrence, long double 
void legendre_reference(double x, int lmax, double *Pref)
{
    long double xl = (long double)x;
    long double *P = malloc((size_t)(lmax + 1) * sizeof(*P));
    if (P == NULL) {
        fprintf(stderr, "allocation failed in legendre_reference\n");
        exit(EXIT_FAILURE);
    }

    P[0] = 1.0L;
    if (lmax >= 1) {
        P[1] = xl;
    }

    for (int l = 1; l < lmax; ++l) {
        P[l + 1] = (((long double)(2 * l + 1)) * xl * P[l]
                    - (long double)l * P[l - 1]) / (long double)(l + 1);
    }

    for (int l = 0; l <= lmax; ++l) {
        Pref[l] = (double)P[l];
    }

    free(P);
}

// Miller-style backward recurrence
void legendre_backward_experiment(double x, int lmax, int L, double *P)
{
    if (L <= lmax) {
        fprintf(stderr, "need L > lmax in legendre_backward_experiment\n");
        exit(EXIT_FAILURE);
    }

    double *Q = calloc((size_t)(L + 2), sizeof(*Q));
    if (Q == NULL) {
        fprintf(stderr, "allocation failed in legendre_backward_experiment\n");
        exit(EXIT_FAILURE);
    }

    Q[L + 1] = 0.0;
    Q[L] = 1.0;

    // P_{l-1}(x) = ((2l+1) x P_l(x) - (l+1) P_{l+1}(x)) / l
    for (int l = L; l >= 1; --l) {
        Q[l - 1] = ((2.0 * l + 1.0) * x * Q[l]
                    - (double)(l + 1) * Q[l + 1]) / (double)l;
    }

    double scale = 1.0 / Q[0];
    for (int l = 0; l <= lmax; ++l) {
        P[l] = scale * Q[l];
    }

    free(Q);
}

static double relative_error(double value, double reference)
{
    double abs_err = fabs(value - reference);
    if (fabs(reference) > DBL_MIN) {
        return abs_err / fabs(reference);
    }
    return abs_err;
}

int main(void)
{
    const int lmax = 50;
    const int L = 80;
    const double xs[] = {0.1, 0.5, 0.9, 0.99};
    const int nx = (int)(sizeof(xs) / sizeof(xs[0]));

    double Pf[51];
    double Pb[51];
    double Pref[51];

    const char *fileout = "legendre_data.dat";
    FILE *fout = fopen(fileout, "w");
    if (fout == NULL) {
        fprintf(stderr, "cannot open %s for writing\n", fileout);
        return EXIT_FAILURE;
    }

    fprintf(fout, "# x l P_ref P_forward P_backward_exp ");
    fprintf(fout, "abs_err_forward rel_err_forward abs_err_backward rel_err_backward\n");

    printf("Legendre recurrence stability (lmax=%d, backward start L=%d)\n\n", lmax, L);
    printf("%-6s %14s %14s %14s %14s\n",
           "x", "max|abs_f|", "max|rel_f|", "max|abs_b|", "max|rel_b|");

    for (int ix = 0; ix < nx; ++ix) {
        double x = xs[ix];

        legendre_reference(x, lmax, Pref);
        legendre_forward(x, lmax, Pf);
        legendre_backward_experiment(x, lmax, L, Pb);

        double max_abs_f = 0.0, max_rel_f = 0.0, max_abs_b = 0.0, max_rel_b = 0.0;

        for (int l = 0; l <= lmax; ++l) {
            long double abs_err_f = fabs(Pf[l] - Pref[l]);
            long double abs_err_b = fabs(Pb[l] - Pref[l]);
            long double rel_err_f = relative_error(Pf[l], Pref[l]);
            long double rel_err_b = relative_error(Pb[l], Pref[l]);

            if (abs_err_f > max_abs_f) max_abs_f = abs_err_f;
            if (rel_err_f > max_rel_f) max_rel_f = rel_err_f;
            if (abs_err_b > max_abs_b) max_abs_b = abs_err_b;
            if (rel_err_b > max_rel_b) max_rel_b = rel_err_b;

            fprintf(fout, "%.6g %d %.15e %.15e %.15e %.15Le %.15Le %.15Le %.15Le\n",
                    x, l, Pref[l], Pf[l], Pb[l],
                    abs_err_f, rel_err_f, abs_err_b, rel_err_b);
        }

	// Blank line to separate the four P_l(x)
        fprintf(fout, "\n\n"); 
        // Print on terminal maximum errors
        printf("%-6.2f %14.3e %14.3e %14.3e %14.3e\n",
               x, max_abs_f, max_rel_f, max_abs_b, max_rel_b);
    }

    fclose(fout);
    printf("\nFull table written to '%s'\n", fileout);

    return 0;
}
