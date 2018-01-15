
x = load(pulse);

ykk = load(output);

ykk2 = ykk(250:(length(x)+249),2);



fitlm(x,ykk2)


quit
