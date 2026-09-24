# Homework 3

The program computes the matrix multiplication:

$$
C=AB\to C_{ij}=\sum_{k}A_{ik}B_{jk}
$$

The programs allow the user to specify from terminal:

- The size $ N*N $ of the matricies A and B
- The value of all the elements of A and B
- The output file in which the matrix would be written

2 different loops for the algorithm are tested:
- a naive "ijk" loop
- the cache optimized "ikj" loop

and their computing time is benchmarked

Also 2 checking algoritms are tested and benchmarked:
- the naive element by element checker (with fix value calculation in the loop)
- a slightly optimized without branching and with only 1 operation in the loop
