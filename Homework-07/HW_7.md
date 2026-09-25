# Homework 6
The C program solves in sequence the proposed problems:

1. **Coin tosses and LLN**  
   Generate 100,000 fair coin tosses with the LCG.

2. **Monte Carlo estimate of $\pi$**  
   Use two LCG streams to estimate $\pi$ for several sample sizes.

3. **Change of variables: $Y = U^2$**  
   Generate $U \sim \mathrm{Uniform}(0,1)$ with the LCG, transform to $Y = U^2$.

4. **Inverse transform exponential**  
   Use the formula $Y=-\ln(1-U)/\lambda$ with $\lambda=1.5$ to generate an exponential variable from the LCG output. Compare histogram and PDF.

5. **Empirical CDF**  
   For the exponential sample from Exercise 4, draw the empirical CDF and compare it to the exact CDF.

## Discussion

1. The fraction of heads to tails converges to 0.5 as $1/\sqrt{N}$, as expected.

2. $\pi$ is computed as the area of a unit circle. The error decreses rroughly as $1/\sqrt{N}$.
   
3. The change of variable from the uniform distribution $U$ to $Y=U^2$ gives the exepcted pdf $f=\frac{1}{2 \sqrt{y}}.
   
4. The computed exponential dsitribution agrees with expected pdf.
   
5. The cumulant distirbution function computed agrees with the expected one $F=1-e^{- \lambda y}$
 
