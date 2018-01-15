
cd single_input/
make clean
make -j3
cp RRN ..
cd ..


sed "s/N/(1000+300)/g" single_input/gen_pulse.m | sed "s/x0/1000/g" > gen_pulse.m

octave -q gen_pulse.m

./RRN pulse.y 

mkdir -p trained_nonoise

mv X.dat trained_nonoise/X.dat
mv Xtarget.dat trained_nonoise/Xtarget.dat
mv output.xy trained_nonoise/output.xy
mv SaveFile.dat trained_nonoise/SaveFile.dat






for i in {1..20}
do

noise=$(echo  "10^( (($i-1)/19)*(3+1) -3)"  | octave -q | cut -d"=" -f2 )

mkdir -p trained_nonoise/I0$i
echo $noise > trained_nonoise/I0$i/I0
    for j in {1..10}
    do
	
	./RRN pulse.y --notrain --nosave --load trained_nonoise/SaveFile.dat --noiseAmp $noise

	mv X.dat trained_nonoise/I0$i/X$j.dat
	mv Xtarget.dat trained_nonoise/I0$i/Xtarget$j.dat
	mv output.xy trained_nonoise/I0$i/output$j.xy
    done

done





./RRN pulse.y  --noiseAmp 0.15

mkdir -p trained_noise

mv X.dat trained_noise/X.dat
mv Xtarget.dat trained_noise/Xtarget.dat
mv output.xy trained_noise/output.xy
mv SaveFile.dat trained_noise/SaveFile.dat




for i in {1..20}
do
noise=$(echo  "10^( (($i-1)/19)*(3+1) -3)"  | octave -q | cut -d"=" -f2)
mkdir -p trained_noise/I0$i
echo $noise > trained_noise/I0$i/I0
    for j in {1..10}
    do
	
	./RRN pulse.y --notrain --nosave --load trained_noise/SaveFile.dat --noiseAmp $noise

	mv X.dat trained_noise/I0$i/X$j.dat
	mv Xtarget.dat trained_noise/I0$i/Xtarget$j.dat
	mv output.xy trained_noise/I0$i/output$j.xy
    done

done







./RRN pulse.y  --notrain 

mkdir -p non_trained

mv X.dat non_trained/X.dat
mv Xtarget.dat non_trained/Xtarget.dat
mv output.xy non_trained/output.xy
mv SaveFile.dat non_trained/SaveFile.dat




for i in {1..20}
do
noise=$(echo  "10^( (($i-1)/19)*(3+1) -3)"  | octave -q | cut -d"=" -f2)
mkdir -p non_trained/I0$i
echo $noise > non_trained/I0$i/I0
    for j in {1..10}
    do
	
	./RRN pulse.y --notrain --nosave --load non_trained/SaveFile.dat --noiseAmp $noise

	mv X.dat non_trained/I0$i/X$j.dat
	mv Xtarget.dat non_trained/I0$i/Xtarget$j.dat
	mv output.xy non_trained/I0$i/output$j.xy
    done

done



rm gen_pulse.m 



