




for i in {200..8200..500}
do
R2=0
for j in {1..5}
do
sed "s/pulse/\'T$i\/pulse.y\'/" R2.m |sed "s/output/\'T$i\/trial\/output$j.xy\'/" >R2temp.m


R2t=$(matlab -r R2temp -nodesktop -nojvm -nosplash -nodisplay  | grep "R-squared" | cut -d":" -f 2 | cut -d"," -f1)

R2= $(echo "$R2t+$R2" | bc -l)
done

echo $i $(echo "$R2/5" | bc -l)

rm R2temp.m
done
