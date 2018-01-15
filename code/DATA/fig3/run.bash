
cd single_input/
make clean
make -j9
cp RRN ..
cd ..



for i in {200..8200..500}
do
    sed "s/N/($i+300)/g" single_input/gen_pulse.m | sed "s/x0/$i/g" > gen_pulse.m

    octave gen_pulse.m

    mkdir -p T$i
    ./RRN pulse.y
    mv X.dat T$i/X_trained.dat
    mv Xtarget.dat T$i/Xtarget_trained.dat
    mv output.xy T$i/output_trained.xy
    mv SaveFile.dat T$i/SaveFile_trained.dat
    mv pulse.y T$i/pulse.y

    mkdir -p T$i/trial


    for j in {1..5}
    do                                          
	./RRN T$i/pulse.y --notrain --nosave --load T$i/SaveFile_trained.dat
	mv X.dat T$i/trial/X$j.dat
	mv output.xy T$i/trial/output$j.xy
    done

done

rm gen_pulse.m


mkdir -p ../../figures

gnuplot dibuja.gnu 

cp *.png ../../figures
