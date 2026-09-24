# Homework 3

The program computes the matrix multiplication:

$$
C=AB\to C_{ij}=\sum_{k}A_{ik}B_{jk}
$$

The programs is run inserting:

- The size $ N*N $ of the matrices 
- The values a and b for the diagonal matrices A B
- The output file name for the matrix C

2 different loops for the algorithm are tested:
- a naive "ijk" loop
- the cache optimized "ikj" loop

and their computing time is benchmarked

Also 2 checking algoritms are tested and benchmarked:
- the naive element by element checker (with fix value calculation in the loop)
- a slightly optimized without branching and with only 1 operation in the loop
