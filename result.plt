# gnuplot script
set terminal gif animate delay 10 enhanced
set output 'cosmos-result.gif'
set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set size 0.75, 0.8
set origin 0.2, 0.15

# file names
POPFILE="pops-result.dat"
MCFILE="data/mcmcdist.csv"

# read axix ranges from data output
LIMITS=system("head -n 2 " . POPFILE . " | tail -n 1")
P1MIN=real(word(LIMITS, 1))
P1MAX=real(word(LIMITS, 2))
P2MIN=real(word(LIMITS, 3))
P2MAX=real(word(LIMITS, 4))
# read population data dimensions from data output
POPSIZE=int(system("head -n 1 " . POPFILE))
NGEN=(int(system("wc " . POPFILE . " | awk '{print $1}'")) - 1)/POPSIZE

unset key

set ylabel "Ω_m"
set xlabel "h_0"
set xtics P1MIN, (P1MAX-P1MIN)/5, P1MAX
set ytics P2MIN, (P2MAX-P2MIN)/5, P2MAX
set xrange[P1MIN:P1MAX]
set yrange[P2MIN:P2MAX]

do for [i=1:NGEN] {
    set title sprintf("Generation = %d", i) offset 10, -3

    set contour base
    set view map
    unset surface
    set cntrparam levels auto 3 # Modify this to your liking

    set table "contours.dat"
    splot MCFILE u 1:2:3 with lines not
    unset table

    #splot MCFILE using 1:2:3 with lines
    unset view
    unset contour
    plot POPFILE every ::POPSIZE*(i-1)+2::POPSIZE*i+1 using 1:2 pt 7 ps 1 not, "contours.dat" u 1:2  w l
}
