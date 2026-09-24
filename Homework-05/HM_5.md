# HomeWork 05 — Stability of Legendre Polynomials

Code: `legendre_stability.c` (forward recurrence, long-double reference,
Miller-style backward experiment) → `legendre_data.dat` → `plot_errors.gp`
(gnuplot) → `legendre_abs_error.png`, `legendre_rel_error.png`.

Parameters used: `lmax = 50`, backward start index `L = 80`,
`x = 0.1, 0.5, 0.9, 0.99`.

## Answers

- For all four values of `x` and all `l = 0..50`, the double-precision
forward recurrence agrees with the long-double reference to within a few
units of machine epsilon.

-  A three-term recurrence has a two-dimensional
  solution space. For Bonnet's recurrence the two independent solutions are the
  Legendre functions of the **first kind** `P_l(x)` and of the **second kind**
  `Q_l(x)`.

- A linear three-term recurrence has two linearly independent solutions: at
each `l` the general solution is a combination of a "dominant" and a
"minimal" solution. Miller's algorithm recovers the minimal solution
reliably: propagating *backward* from an arbitrary starting pair works
because, in the backward direction, the minimal solution of the original
recurrence becomes the dominant one, so any small admixture of the unwanted
solution is exponentially damped out by the time you reach small `l`. That
trick only works when the two solutions are genuinely separated into a
dominant and a minimal branch relative to the propagation direction.

- For ordinary Legendre polynomials `P_l(x)` on `|x| < 1`, that separation
does not hold: both linearly independent solutions of Bonnet's recurrence
(the polynomial `P_l` and the second-kind solution `Q_l`)
stay comparable in size along the recurrence — neither is exponentially
suppressed relative to the other in the backward direction. So starting
Miller's procedure at an arbitrary `(P̃_{L+1}, P̃_L)` does not preferentially
select `P_l(x)`; it selects a combination that is *not* the Legendre
sequence. That is exactly what the data show: consistent O(1)-scale disagreement at every `l`, not the convergence.

- The forward recurrence is the right (and stable) tool for
computing ordinary `P_l(x)` on `|x| < 1`; the backward/Miller approach would be the right tool only for a recurrence where the
desired solution is genuinely the minimal one in the backward direction.

**Connection to spherical harmonics.**
For `m = 0`,

$$Y_{l0}(θ, φ) = \sqrt{\frac{2l+1}{4π}} · P_l(cos θ)$$

so any error in `P_l(cos θ)` propagates directly and linearly into
`Y_{l0}`, scaled by the normalization factor
`sqrt((2l+1)/(4π))`. Since the forward recurrence keeps `P_l` accurate to
roundoff, `Y_{l0}` computed this way is likewise accurate to roundoff.
