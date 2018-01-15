

cd single_input/
octave gen_pulse.m
cp pulse.y ..
make clean
make -j3
cp RRN ..
cd ..


mkdir -p trained

./RRN pulse.y --pconnect 0.25

mv X.dat trained/X.dat
mv Xtarget.dat trained/Xtarget.dat
mv output.xy trained/output.xy
mv SaveFile.dat trained/SaveFile.dat


mkdir -p trained/trial

for i in {1..10}
do

./RRN pulse.y --notrain --nosave --noiseAmp 0.15 --load trained/SaveFile.dat

mv X.dat trained/trial/X$i.dat
mv Xtarget.dat trained/trial/Xtarget$i.dat
mv output.xy trained/trial/output$i.xy

done


###################################################

mkdir -p non-trained

./RRN pulse.y --notrain --pconnect 0.25

mv X.dat non-trained/X.dat
mv Xtarget.dat non-trained/Xtarget.dat
mv output.xy non-trained/output.xy
mv SaveFile.dat non-trained/SaveFile.dat


mkdir -p non-trained/trial

for i in {1..10}
do

./RRN pulse.y --notrain --nosave --noiseAmp 0.15 --load non-trained/SaveFile.dat

mv X.dat non-trained/trial/X$i.dat
mv Xtarget.dat non-trained/trial/Xtarget$i.dat
mv output.xy non-trained/trial/output$i.xy

done


g++ -O3 -std=c++11 var.cpp -o var

./var > trained/trained.var
./var non > non-trained/non-trained.var


mkdir -p ../../figures

gnuplot dibuja.gnu

cp *.png ../../figures

rm var






