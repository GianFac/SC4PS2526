#!/usr/bin/env gnuplot
#
# HomeWork 07 
#
# Run with:  gnuplot plot_hw07.gp

# ---------------------------------------------------------------------
# Exercise 1: running mean fraction of heads vs n   (log x)
# ---------------------------------------------------------------------
reset
set datafile separator ","
set terminal pngcairo size 900,650 enhanced font "Helvetica,12"
set grid lt 1 lc rgb "#dddddd"

set output "plots/Ex1_plot.png"
set title "Mean heads fraction"
set xlabel "n events"
set ylabel "heads fraction"
set logscale x
set key top right
plot "data/Ex1.csv" every ::1 using 1:($2/$1) with lines lw 1.5 lc rgb "blue" title "Mean heads fraction"

# ---------------------------------------------------------------------
# Exercise 2: |pi_estimate - pi| vs n   (log-log)
# ---------------------------------------------------------------------
reset
set datafile separator ","
set terminal pngcairo size 900,650 enhanced font "Helvetica,12"
set grid lt 1 lc rgb "#dddddd"

set output "plots/Ex2_plot.png"
set title "Error pi estimate"
set xlabel "n events"
set ylabel "Error"
set logscale x
set logscale y
set key top right
plot "data/Ex2.csv" every ::1 using 1:(abs($2-pi)) with linespoints lw 1.5 pt 7 ps 0.8 lc rgb "blue" title "Error in pi estimate"

# ---------------------------------------------------------------------
# Exercise 3: histogram of Y = U^2 vs analytic density 1/(2*sqrt(y))
# ---------------------------------------------------------------------
reset
set datafile separator ","
set terminal pngcairo size 900,650 enhanced font "Helvetica,12"
set grid lt 1 lc rgb "#dddddd"

stats "Ex3.csv" every ::1 using 1 nooutput
n3 = STATS_records
min3 = STATS_min
max3 = STATS_max
binwidth3 = (max3 - min3) / 200.0
bin(x,w,lo) = w*floor((x-lo)/w) + lo + w/2.0

set output "plots/Ex3_plot.png"
set title "Distribution of y = u^2"
set xlabel "y values"
set ylabel "Frequency"
set boxwidth binwidth3
set style fill solid 0.6 border lc rgb "black"
set key top right
plot "data/Ex3.csv" every ::1 using (bin($1,binwidth3,min3)):(1.0/(n3*binwidth3)) \
        smooth freq with boxes lc rgb "blue" title "Y = U^2", \
     (x >= 0.005 && x <= 1.0) ? 1/(2*sqrt(x)) : 1/0 \
        with lines lc rgb "red" lw 2 title "1/(2{/Symbol \326}y)"

# ---------------------------------------------------------------------
# Exercise 4: histogram of Y = -ln(1-U)/1.5 vs analytic density
# ---------------------------------------------------------------------
reset
set datafile separator ","
set terminal pngcairo size 900,650 enhanced font "Helvetica,12"
set grid lt 1 lc rgb "#dddddd"

stats "Ex4_5.csv" every ::1 using 1 nooutput
n4 = STATS_records
min4 = STATS_min
max4 = STATS_max
binwidth4 = (max4 - min4) / 200.0
bin(x,w,lo) = w*floor((x-lo)/w) + lo + w/2.0

set output "plots/Ex4_plot.png"
set title "Distribution of y = -log(1-U)/1.5"
set xlabel "y values"
set ylabel "Frequency"
set boxwidth binwidth4
set style fill solid 0.6 border lc rgb "black"
set key top right
plot "data/Ex4_5.csv" every ::1 using (bin($1,binwidth4,min4)):(1.0/(n4*binwidth4)) \
        smooth freq with boxes lc rgb "blue" title "Y = -log(1-U)/1.5", \
     (x >= 0.005 && x <= 4.0) ? 1.5*exp(-1.5*x) : 1/0 \
        with lines lc rgb "red" lw 2 title "1.5 e^{-1.5y}"

# ---------------------------------------------------------------------
# Exercise 5: empirical CDF vs theoretical CDF 1 - exp(-1.5 y)
# ---------------------------------------------------------------------
reset
set datafile separator ","
set terminal pngcairo size 900,650 enhanced font "Helvetica,12"
set grid lt 1 lc rgb "#dddddd"

set output "plots/Ex5_plot.png"
set title "Distribution of y = -log(1-U)/1.5"
set xlabel "y values"
set ylabel "Frequency"
set key bottom right
plot "data/Ex4_5.csv" every ::1 using 2:3 with lines lc rgb "blue" lw 2 title "Empirical CDF", \
     (x >= 0.001 && x <= 8.0) ? 1.0 - exp(-1.5*x) : 1/0 \
        with lines lc rgb "red" dashtype 2 lw 2 title "Theoretical CDF"

print "Wrote Ex1_plot.png .. Ex5_plot.png"
