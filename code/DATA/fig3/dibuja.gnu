

set term pngcairo size 1200,600 enhanced font 'Verdana,20'


##############################Output#################
set xlabel "Time(s)"
set ylabel "Output"

set border lw 3


set output "fig3a2.png"
set key top center
set arrow 3 from 0,-0.5 to 0,1.1 nohead lw 5
set arrow 4 from 2.5,-0.5 to 2.5,1.1 nohead lw 3
plot[-0.25:3][-0.5:1.1] "T2200/pulse.y" u ($0/1000):1 w l lw 3 lc 1 title "Target", "T2200/output_trained.xy" u (($1-250)/1000):2 w l lw 4 lc 3 notitle

unset arrow 4

set output "fig3a3.png"
set arrow 1 from 0,-0.5 to 0,1.1 nohead lw 5  
set arrow 2 from 6.5,-0.5 to 6.5,1.1 nohead lw 3
plot[-0.25:7][-0.5:1.1] "T6200/pulse.y" u ($0/1000):1 w l lw 3 lc 1 title "Target", "T6200/output_trained.xy" u (($1-250)/1000):2 w l lw 4 lc 3 notitle



unset arrow 1
unset arrow 2
unset arrow 3

set xlabel "Interval (s)"
set ylabel "Performance (R^2)"

set output "fig3b.png"
plot[][-0.1:1.1] "R2vsT.dat" u ($1/1000):2 w lp lw 3 notitle

