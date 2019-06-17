# gnuplot script
set terminal gif animate delay 10 enhanced
set output 'cosmos.gif'
set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set size 1.0, 1.0
set origin 0.0, 0.0

# file names
POPFILE="pops.dat"

# read axix ranges from data output
LIMITS=system("head -n 2 " . POPFILE . " | tail -n 1")
P1MIN=real(word(LIMITS, 1))
P1MAX=real(word(LIMITS, 2))
P2MIN=real(word(LIMITS, 3))
P2MAX=real(word(LIMITS, 4))
P3MIN=real(word(LIMITS, 5))
P3MAX=real(word(LIMITS, 6))
# read population data dimensions from data output
POPSIZE=int(system("head -n 1 " . POPFILE))
NGEN=(int(system("wc " . POPFILE . " | awk '{print $1}'")) - 1)/POPSIZE

do for [i=1:NGEN] {
    set multiplot layout 3,1 title sprintf("Generation = %d", i) offset 20,-1
    unset key

    set size 0.4, 0.4
    set origin 0.15, 0.15
    set xtics P1MIN, (P1MAX-P1MIN)/5, (P1MAX*4+P1MIN)/5
    set ytics P3MIN, (P3MAX-P3MIN)/5, (P3MAX*4+P3MIN)/5
    set xlabel "h_0"
    set ylabel "Ω_r"
    set xrange[P1MIN:P1MAX]
    set yrange[P3MIN:P3MAX]
    plot POPFILE every ::POPSIZE*(i-1)+2::POPSIZE*i+1 using 1:3

    set size 0.4, 0.4
    set origin 0.55, 0.15
    set xlabel "Ω_m"
    unset ylabel
    set xtics P2MIN, (P2MAX-P2MIN)/5, (P2MAX*4+P2MIN)/5
    unset ytics
    set xrange[P2MIN:P2MAX]
    set yrange[P3MIN:P3MAX]
    plot POPFILE every ::POPSIZE*(i-1)+2::POPSIZE*i+1 using 2:3

    set size 0.4, 0.4
    set origin 0.15, 0.55
    set ylabel "Ω_m"
    unset xlabel
    set ytics P2MIN, (P2MAX-P2MIN)/5, (P2MAX*4+P2MIN)/5
    unset xtics
    set xrange[P1MIN:P1MAX]
    set yrange[P2MIN:P2MAX]
    plot POPFILE  every ::POPSIZE*(i-1)+2::POPSIZE*i+1 using 1:2

    unset multiplot
}
