

cd GfxTablet/driver-uinput/
make
sudo ./networktablet > /dev/null &
cd ../../
make -j2

echo "You may need to change MOUSEEVENT in parse_tablet_event.cpp!!"
echo "Now write your word using the tablet and press Ctrl-C!"
trap : INT

sudo ./parse_tablet_event > word.xy

./parse_and_interpolate word.xy > word.xy.filtered


sudo killall networktablet
