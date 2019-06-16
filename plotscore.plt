# gnuplot script
set terminal png
set xlabel "generations"
set output "scores.png"
set key right center
plot "score.dat" title "scores" with histeps linewidth 3
