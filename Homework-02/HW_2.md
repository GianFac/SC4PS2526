# Homework 2

The program computes the vector sum:

$$
\vec{d} = a \vec{x} + \vec{y}
$$

When running the program you need to input:
- The size $N$ of the vectors $\vec{x}$ and $\vec{y}$  
- The scalar $a$  
- The value of all the equal elements of $\vec{x}$ and $\vec{y}$
- The expecte value for $a \vec{x} + \vec{y}$  

The program allocates the memory dynamically to handle vectors of large size N.

The programs tests each elements of the computed vector sum against the exepcted value computed manualy inserted.

The test never passes for `a=3, x=0.1, y=7.1` becasue 0.1 and 7.1 cannot be exactly represented in the floating point format.\
For example, putting `a=3, x=0.25, y=7.5` gives correctly `d=8.25` because the decimal part can be written as binary fractions.
