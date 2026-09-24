# Homework 6

The program computes the vector sum:

$$
\vec{d} = a \vec{x} + \vec{y}
$$

following the method of `Homework-02` and also by "chunks" smaller than the vector length.

During the calculation of the vector $\vec{d}$, the sum of all its elements is also computed, so that it is calculated by summing directly all the elemnts, ore summin the partial sums of the smaller chunks.

The `config.txt` defines the paramters for the program:

- The size $N$ of the vectors 
- The scalar value $a$  
- The value assigned to all elements of $\vec{x}$ and $\vec{y}$ 
- The chuck size 

The two vectors are tested against each other with exact equality `==`. They are always the same because the computation of each element is exactly the same.

The total sum however is not always the same between the two methods because of floating-point accuracy. Direct summation accumulated more rounding error than the partial sums.

The resulting vector and the sum of the various chucks is then printend in a HDF5 file `results.h5`
