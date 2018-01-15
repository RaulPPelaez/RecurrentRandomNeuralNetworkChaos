

set term pngcairo size 1200,600 enhanced font 'Verdana,20'


##############################Output#################
set xlabel "Time(s)"
set ylabel "Output"

set border lw 3


set output "fig4a_out.png"
set key top center
set arrow 3 from 0,-0.5 to 0,1.1 nohead lw 5
set arrow 4 from 1.5,-0.5 to 1.5,1.1 nohead lw 3
plot[-0.25:1.25][-0.5:1.1] for[i=1:2] "trained/trial/output".i.".xy" u ($1/1000-0.25):2 w l lw 3 lc (i==1?1:3) notitle

#########################variance########################
set arrow 3 from 0,0 to 0,0.35 nohead lw 5
set arrow 4 from 1.5,0 to 1.5,0.35 nohead lw 3
set output "fig4a_var.png"
set ylabel "Variance"
set termop dashed
plot[-0.25:1.5][0:0.35] "trained/trained.var" u ($1/1000-0.2):2 w l lw 3 lc 3 lt 1 title "Post-train", "non-trained/non-trained.var" u ($1/1000-0.2):2 w l lw 3 lc 3 lt 0 title "Pre-train"


##############################Units#################

set output "fig4a1.png"

set arrow 3 from 0,-11 to 0,-3 nohead lw 5
set arrow 4 from 1.5,-11 to 1.5,-3 nohead lw 3

unset ytics
set termop solid
set ylabel "Firing rate"
plot[-0.25:1.5] for[i=2:5] "trained/trial/X1.dat" u (($1-200)/1000):(column(i)-2*i) w l lw 3 lc 1 notitle, for[i=2:5] "trained/trial/X6.dat" u (($1-200)/1000):(column(i)-2*i) w l lw 3 lc 3 notitle


