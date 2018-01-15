

set term pngcairo size 1200,600 enhanced font 'Verdana,20'


##############################Output#################
set xlabel "Time(s)"
set ylabel "Output"

set border lw 3


set output "fig1b2_Z.png"
set arrow from 0,-2 to 0,2 nohead lw 5
set arrow from 2.2,-2 to 2.2,2 nohead lw 3
plot[-0.25:3][-2:2] for[i=1:5] "trained/trial/output".i.".xy" u (($1-200)/1000):2 w l lw 4 notitle

set output "fig1b1_Z.png"
set arrow 1 from 0,-2 to 0,2 nohead lw 5  
set arrow 2 from 2.2,-2 to 2.2,2 nohead lw 3
plot[-0.25:3][-2:2] for[i=1:5] "non_trained/trial/output".i.".xy" u (($1-200)/1000):2 w l lw 4 notitle

unset arrow 1
unset arrow 2

set termoption dashed
set output "fig1c2_Z.png"
set arrow from 0,-0.5 to 0,1.1 nohead lw 5
set arrow from 0.5,-0.5 to 0.5,1.1 nohead lw 3 lt 0

plot[-0.25:2.25][-0.5:1.1] for[i=1:5] "trained/perturbed/output".i.".xy" u (($1-200)/1000):2 w l lw 4 lt 1 lc i notitle

set output "fig1c1_Z.png"
set arrow from 0,-2 to 0,2 nohead lw 5
set arrow from 2.2,-2 to 2.2,2 nohead lw 3
set arrow from 0.5,-2 to 0.5,2 nohead lw 3 lt 0

plot[-0.25:3][-2:2] for[i=1:5] "non_trained/perturbed/output".i.".xy" u (($1-200)/1000):2 w l lw 4 lt 1 lc i notitle

##################RATES###################################

set termoption solid



set ylabel "Figring rates"
unset ytics

set output "fig1b2_rates.png"
set arrow from 0,-2 to 0,2 nohead lw 5
set arrow from 2.2,-2 to 2.2,2 nohead lw 3

plot[-0.2:3] for[i=2:5] "trained/trial/X1.dat" u (($1-250)/1000):(column(i)-2*i) w l lw 4 lc 1 notitle, for[i=2:5] "trained/trial/X2.dat" u (($1-250)/1000):(column(i)-2*i) w l lw 4 lc 3 notitle


set output "fig1b1_rates.png"
plot[-0.2:3] for[i=2:5] "non_trained/trial/X1.dat" u (($1-250)/1000):(column(i)-2*i) w l lw 4 lc 1 notitle, for[i=2:5] "non_trained/trial/X2.dat" u (($1-250)/1000):(column(i)-2*i) w l lw 4 lc 3 notitle
