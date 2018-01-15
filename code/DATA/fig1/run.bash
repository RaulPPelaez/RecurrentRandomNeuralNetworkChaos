export OMP_NUM_THREADS=2


cd single_input/
make clean
make -j3
octave gen_pulse.y
cp pulse.y ..
cd ..

cp single_input/RRN .


mkdir -p non_trained

./RRN pulse.y --notrain  
mv X.dat non_trained/X$i.dat
mv Xtarget.dat non_trained/Xtarget$i.dat
mv output.xy non_trained/output$i.xy
mv SaveFile.dat non_trained/SaveFile.dat


mkdir -p non_trained/trial

for i in {1..5}
do

./RRN pulse.y --notrain  --nosave --load non_trained/SaveFile.dat

mv X.dat non_trained/trial/X$i.dat
mv output.xy non_trained/trial/output$i.xy
done




mkdir -p trained

./RRN pulse.y
mv X.dat trained/X.dat
mv Xtarget.dat trained/Xtarget.dat
mv output.xy trained/output.xy
mv SaveFile.dat trained/SaveFile.dat


mkdir -p trained/trial
for i in {1..5}
do
./RRN pulse.y --notrain  --nosave --load trained/SaveFile.dat

mv X.dat trained/trial/X$i.dat
mv output.xy trained/trial/output$i.xy
done







mkdir -p trained/perturbed

for i in {1..5}
do

./RRN pulse.y --notrain  --nosave --perturbation --load trained/SaveFile.dat

mv X.dat trained/perturbed/X$i.dat
mv output.xy trained/perturbed/output$i.xy
done



mkdir -p non_trained/perturbed

for i in {1..5}
do

./RRN pulse.y --notrain  --nosave --perturbation --load non_trained/SaveFile.dat

mv X.dat non_trained/perturbed/X$i.dat
mv output.xy non_trained/perturbed/output$i.xy
done


mkdir -p ../../figures

gnuplot dibuja.gnu

mv *.png ../../figures






