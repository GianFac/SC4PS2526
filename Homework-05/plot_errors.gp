#!/usr/bin/env gnuplot
#
# HomeWork 05 - Stability of Legendre Polynomials
# Reads legendre_data.dat (produced by legendre_stability.c) and makes
# two figures, each a 2x2 grid (one per x value):
#
#   legendre_abs_error.png  -> |P_l - P_ref| vs l   (forward & backward)
#   legendre_rel_error.png  -> relative error vs l  (forward & backward)
#
# Run with:  gnuplot plot_errors.gp

datafile = "legendre_data.dat"

# x values in the same order they were written to the data file
array xval[4]
xval[1] = "0.1"
xval[2] = "0.5"
xval[3] = "0.9"
xval[4] = "0.99"

set terminal pngcairo size 1200,900 enhanced font "Helvetica,11"
set grid lt 1 lc rgb "#dddddd"
set key top left box opaque
set xlabel "L"
set xrange [0:50]

# ---------------------------------------------------------------------
# Figure 1: absolute error
# ---------------------------------------------------------------------
set output "legendre_abs_error.png"
set multiplot layout 2,2 title "Legendre P_l(x): absolute error vs l (forward vs backward)" font "Helvetica,14"

set logscale y
set format y "10^{%L}"
set ylabel "|P_l - P_{ref}|"
set key center left

do for [i=0:3] {
    set title sprintf("x = %s", xval[i+1])
    plot datafile index i using 2:6 with linespoints pt 7 ps 0.9 lc rgb "#1f77b4" title "forward",  \
         datafile index i using 2:8 with linespoints pt 7 ps 0.9 lc rgb "#d62728" title "backward"
}
unset multiplot
unset logscale y

# ---------------------------------------------------------------------
# Figure 2: relative error
# ---------------------------------------------------------------------
set output "legendre_rel_error.png"
set multiplot layout 2,2 title "Legendre P_l(x): relative error vs l (forward vs backward)" font "Helvetica,14"

set logscale y
set format y "10^{%L}"
set ylabel "relative error"
set key center left

do for [i=0:3] {
    set title sprintf("x = %s", xval[i+1])
    plot datafile index i using 2:7 with linespoints pt 7 ps 0.9 lc rgb "#1f77b4" title "forward",  \
         datafile index i using 2:9 with linespoints pt 7 ps 0.9 lc rgb "#d62728" title "backward"
}
unset multiplot
unset logscale y

print "Wrote legendre_abs_error.png and legendre_rel_error.png"
