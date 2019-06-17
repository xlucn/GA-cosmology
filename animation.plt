# gnuplot script
set terminal gif animate delay 100 enhanced
set output 'cosmos.gif'
set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set size 1.0, 1.0
set origin 0.0, 0.0

NGEN=5
POPSIZE=16

do for [i=1:NGEN] {
    set multiplot layout 3,1 title sprintf("Generation = %d",i) offset 20,-1
    unset key

    set size 0.4, 0.4
    set origin 0.15, 0.15
    set xtics 0.2, 0.2, 0.8
    set ytics 0.2, 0.2, 0.8
    set xlabel "h_0"
    set ylabel "Ω_r"
    set xrange[0:1]
    set yrange[0:1]
    plot "pops.dat" every ::(POPSIZE+1)*(i-1)+2::(POPSIZE+1)*i using 1:3

    set size 0.4, 0.4
    set origin 0.55, 0.15
    set xlabel "Ω_m"
    unset ylabel
    set xtics 0.2, 0.2, 0.8
    unset ytics
    set xrange[0:1]
    set yrange[0:1]
    plot "pops.dat" every ::(POPSIZE+1)*(i-1)+2::(POPSIZE+1)*i using 2:3

    set size 0.4, 0.4
    set origin 0.15, 0.55
    set ylabel "Ω_m"
    unset xlabel
    set ytics 0.2, 0.2, 0.8
    unset xtics
    set xrange[0:1]
    set yrange[0:1]
    plot "pops.dat" every ::(POPSIZE+1)*(i-1)+2::(POPSIZE+1)*i using 1:2

    unset multiplot
}
