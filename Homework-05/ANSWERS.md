# HomeWork 05 — Stability of Legendre Polynomials

Code: `legendre_stability.c` (forward recurrence, long-double reference,
Miller-style backward experiment) → `legendre_data.dat` → `plot_errors.gp`
(gnuplot) → `legendre_abs_error.png`, `legendre_rel_error.png`.

Parameters used: `lmax = 50`, backward start index `L = 80`,
`x = 0.1, 0.5, 0.9, 0.99`.

## Results

| x | max abs err, forward | max rel err, forward | max abs err, backward | max rel err, backward |
|---|---|---|---|---|
| 0.10 | 2.8e‑17 | 6.9e‑15 | 2.48 | 2.8e+02 |
| 0.50 | 5.6e‑17 | 8.1e‑16 | 7.88 | 4.7e+01 |
| 0.90 | 2.5e‑16 | 5.6e‑15 | 0.12 | 1.6 |
| 0.99 | 4.7e‑16 | 1.1e‑14 | 0.25 | 2.9 |

See `legendre_abs_error.png` and `legendre_rel_error.png` (2×2 grid, one panel
per x, forward vs. backward). Note the ~15–16 orders of magnitude gap between
the two curves in every panel.

## Answers to the assignment's questions

**Is the forward recurrence stable?**
Yes. For all four values of `x` and all `l = 0..50`, the double-precision
forward recurrence agrees with the long-double reference to within a few
units of machine epsilon (relative error ≲ 1.1e‑14, i.e. essentially
roundoff noise that grows mildly with `l` but never accumulates). Bonnet's
recurrence is therefore stable in the forward direction for ordinary
Legendre polynomials on this range of `x`.

**Does the backward (Miller-style) experiment recover `P_l(x)`?**
No. Starting from the arbitrary pair `P̃_{L+1}=0`, `P̃_L=1` and rescaling so
that `P̃_0 = 1` gives values that differ from `P_l(x)` by O(0.1)–O(10) in
absolute terms and by O(1)–O(100) in relative terms — errors many orders of
magnitude larger than roundoff, and not concentrated at one end of the
range: they persist across all `l`. Repeating the experiment with a much
larger starting index (`L = 300` instead of 80) does **not** systematically
shrink these errors — for some `x` they even get worse — showing the
procedure is not converging to `P_l(x)` as `L` increases.

**Why does this happen?**
A linear three-term recurrence has two linearly independent solutions: at
each `l` the general solution is a combination of a "dominant" and a
"minimal" solution. Miller's algorithm recovers the minimal solution
reliably: propagating *backward* from an arbitrary starting pair works
because, in the backward direction, the minimal solution of the original
recurrence becomes the dominant one, so any small admixture of the unwanted
solution is exponentially damped out by the time you reach small `l`. That
trick only works when the two solutions are genuinely separated into a
dominant and a minimal branch relative to the propagation direction.

For ordinary Legendre polynomials `P_l(x)` on `|x| < 1`, that separation
does not hold: both linearly independent solutions of Bonnet's recurrence
(the polynomial `P_l` and the second-kind solution `Q_l`-like companion)
stay comparable in size along the recurrence — neither is exponentially
suppressed relative to the other in the backward direction. So starting
Miller's procedure at an arbitrary `(P̃_{L+1}, P̃_L)` does not preferentially
select `P_l(x)`; it selects a combination that is *not* the Legendre
sequence, and rescaling by `P̃_0` only fixes one normalization condition —
it cannot repair a wrong linear combination. That is exactly what the
data show: consistent O(1)-scale disagreement at every `l`, not the clean
convergence you would see if the minimal-solution mechanism applied here.

**Conclusion:** the forward recurrence is the right (and stable) tool for
computing ordinary `P_l(x)` on `|x| < 1`; the backward/Miller approach, as
specified in the exercise, is a useful cautionary example rather than a
working method — it would be the right tool only for a recurrence where the
desired solution is genuinely the minimal one in the backward direction
(e.g. certain Bessel-function recurrences), which is not the case here.

**Connection to spherical harmonics.**
For `m = 0`,
```
Y_{l0}(θ, φ) = sqrt((2l+1)/(4π)) · P_l(cos θ)
```
so any error in `P_l(cos θ)` propagates directly and linearly into
`Y_{l0}`, scaled by the known, well-conditioned normalization factor
`sqrt((2l+1)/(4π))`. Since the forward recurrence keeps `P_l` accurate to
roundoff, `Y_{l0}` computed this way is likewise accurate to roundoff;
had the backward experiment been used instead, the O(1) relative errors
seen above would carry straight through to `Y_{l0}`, making it useless.
