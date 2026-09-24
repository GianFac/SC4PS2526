# Homework 2

This folder contains two C programs that compute the vector operation:

$$
\vec{d} = a \vec{x} + \vec{y}
$$

The programs allow the user to specify:

- The size $ N $ of the vectors $ \vec{x} $ and $ \vec{y} $  
- The scalar value $ a $  
- The value assigned to all elements of $ \vec{x} $ and $ \vec{y} $  

Both implementations also verify that the elements of the vector $ \vec{d} $ matches the expected value.

---

### Files:

 `vector_sum_static.c`
- Uses static memory allocation (stack-based arrays)
- Produce a segmentation fault error when $ N $ is large, due to stack size limitations

 `vector_sum_dynamic.c`
- Uses dynamic memory allocation (heap-based arrays)
- Supports much larger values of $ N $, limited only by available system memory
