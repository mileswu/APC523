#!/bin/bash

cat > _temp.gnuplot << EOF

set term X11
set logscale y
set xlabel "Number of iterations"
set ylabel "Error"

f(x) = a * p ** x

fit [0:7000] f(x) 'jacobi_j_35.csv' using 1:2 via a,p

plot 'jacobi_j_35.csv', f(x)

pause -1 "Hit any key to continue"

EOF





gnuplot -e "load \"_temp.gnuplot\""
rm _temp.gnuplot
