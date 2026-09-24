#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   /* strcasecmp */
#include <math.h>
#include <hdf5.h>
#include "parser.h"    /* https://github.com/welljsjs/Config-Parser-C, vendored locally */

/* compile with:
 *   gcc vector_sum_HDF5.c -o vector_sum_HDF5 \
 *       -I/usr/include/hdf5/serial -L/usr/lib/x86_64-linux-gnu/hdf5/serial -lhdf5
 */

/* ---- small helpers built on top of parser.h's linked list ----------- */

static const char *config_lookup(config_option_t co, const char *key) {
    for (config_option_t p = co; p != NULL; p = p->prev) {
        if (strcasecmp(p->key, key) == 0) return p->value;
    }
    return NULL;
}

static int config_get_int(config_option_t co, const char *key, int def) {
    const char *v = config_lookup(co, key);
    return v ? atoi(v) : def;
}

static double config_get_double(config_option_t co, const char *key, double def) {
    const char *v = config_lookup(co, key);
    return v ? atof(v) : def;
}

static void config_free_all(config_option_t co) {
    while (co != NULL) {
        config_option_t prev = co->prev;
        free(co);
        co = prev;
    }
}

int main() {
    /* ---- 3) read n, chunk_size, a, x, y from config.txt via parser.h - */
    config_option_t config = read_config_file("config.txt");
    if (config == NULL) {
        fprintf(stderr, "Error: could not read config.txt "
                        "(expected 'key = value' lines, see parser.h)\n");
        return EXIT_FAILURE;
    }

    int n          = config_get_int(config, "n", 100);
    int chunk_size = config_get_int(config, "chunk_size", 8);
    double a       = config_get_double(config, "a", 2.0);
    double x_val   = config_get_double(config, "x", 1.0);
    double y_val   = config_get_double(config, "y", 2.0);

    config_free_all(config);

    printf("Loaded Config -> n: %d, chunk_size: %d, a: %.2f, x: %.2f, y: %.2f\n",
           n, chunk_size, a, x_val, y_val);

    if (n <= 0 || chunk_size <= 0) {
        fprintf(stderr, "Error: Invalid vector size or chunk size.\n");
        return EXIT_FAILURE;
    }

    /* Allocate memory for vectors */
    double *x = (double *)malloc(n * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));
    double *d = (double *)malloc(n * sizeof(double));
    double *d_orig = (double *)malloc(n * sizeof(double)); /* For validation */

    /* Initialize vectors x and y using the (now actually used) config
     * values x_val, y_val as the per-element scale factors */
    for (int i = 0; i < n; i++) {
        x[i] = x_val;
        y[i] = y_val;
    }

    /* ---- 1) Chunked Vector Sum ------------------------------------- */

    /* Calculate the number of chunks (ceiling division) */
    int n_chunks = (n + chunk_size - 1) / chunk_size;
    double *partial_chunk_sum = (double *)malloc(n_chunks * sizeof(double));

    /* Iterates through chunks */
    for (int chunk_idx = 0; chunk_idx < n_chunks; chunk_idx++) {
        /* Calculate start and end indices for C (0-based indexing) */
        int current_start = chunk_idx * chunk_size;
        int current_end = current_start + chunk_size;

        /* Adjust for the last chunk */
        if (current_end > n) {
            current_end = n;
        }

        double current_chunk_sum = 0.0;

        /* Inner loop: actual processing logic */
        for (int i = current_start; i < current_end; i++) {
            d[i] = a * x[i] + y[i];
            current_chunk_sum += d[i];
        }

        /* ---- 2) Save the sum for this chunk ------------------------ */
        partial_chunk_sum[chunk_idx] = current_chunk_sum;
    }
    double total_partial_sum = 0.0;
    for (int i = 0; i < n_chunks; i++) {
        total_partial_sum += partial_chunk_sum[i];
    }

    /* ---- Validation --------------------------------------------------
     * Compare against the original, unchunked, single-loop computation.
     */
    double total_sum_d_orig = 0.0;
    for (int i = 0; i < n; i++) {
        d_orig[i] = a * x[i] + y[i];
        total_sum_d_orig += d_orig[i];
    }

    /* Element by element */
    int vectors_match = 0;
    for (int i = 0; i < n; i++) {
        if (d[i] != d_orig[i]) {
            vectors_match = 1;
            break;
        }
    }
    if (vectors_match == 0) {
        printf("Element by element comparison: PASSED\n");
    } else {
        printf("Element by element comparison: FAILED\n");
    }

    /* Sum of partial chunks */
    if (total_sum_d_orig == total_partial_sum) {
        printf("Sum of partial chunks equals total sum: PASSED\n");
    } else {
        printf("Sum of partial chunks equals total sum: FAILED\n");
    }
    
    printf("Total sum:\t %.15f\n", total_sum_d_orig);
    printf("Sum of partial chunks:\t %.15f\n", total_partial_sum);

    /* ---- 4) Save vector chunks and partial sums in an HDF5 file ----- */

    hid_t file_id, space_d, space_p, dataset_d, dataset_p, plist_d;
    hsize_t dims_d[1] = {(hsize_t)n};
    hsize_t dims_p[1] = {(hsize_t)n_chunks};

    file_id = H5Fcreate("results.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /* Write full vector 'd', physically stored in HDF5 in the SAME
     * chunks used for the computation above (H5Pset_chunk), rather
     * than as one contiguous block -> this is what actually satisfies
     * "save all vector chunks" at the file-storage level. */
    hsize_t hdf5_chunk = (hsize_t)(chunk_size < n ? chunk_size : n);
    plist_d = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(plist_d, 1, &hdf5_chunk);

    space_d = H5Screate_simple(1, dims_d, NULL);
    dataset_d = H5Dcreate(file_id, "d_vector", H5T_NATIVE_DOUBLE, space_d,
                           H5P_DEFAULT, plist_d, H5P_DEFAULT);
    H5Dwrite(dataset_d, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, d);

    /* Write 'partial_chunk_sum' (one value per chunk) */
    space_p = H5Screate_simple(1, dims_p, NULL);
    dataset_p = H5Dcreate(file_id, "partial_sums", H5T_NATIVE_DOUBLE, space_p,
                           H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dataset_p, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_chunk_sum);

    H5Dclose(dataset_d); H5Sclose(space_d); H5Pclose(plist_d);
    H5Dclose(dataset_p); H5Sclose(space_p);
    H5Fclose(file_id);

    printf("Saved 'd_vector' (chunked, chunk size=%llu) and 'partial_sums' to results.h5\n",
           (unsigned long long)hdf5_chunk);

    /* Clean up memory */
    free(x); free(y); free(d); free(d_orig); free(partial_chunk_sum);

    return 0;
}
