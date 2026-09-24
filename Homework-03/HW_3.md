# Homework 3

The program computes the matrix multiplication:

$$
C=AB\to C_{ij}=\sum_{k}A_{ik}B_{jk}
$$

The programs is run inserting:

- The size $N*N$ of the matrices 
- The values a and b for the diagonal matrices $A$ $B$
- The output file name for the matrix $C$

The program compares the computation time of the ijk loop vs the ikj loop.

The output matrix values are tested with a threshold against the value $c$ computed only once.\
One function compares all the elements of the matrix, while the other only checks the diagonal since $C$ is the product of two diaganl matrices.
