




x = 0:2200;
bell = normpdf(x, 2000, 30);

Z = 0.2+(  (1-0.2)/max(bell)   )*bell;
Z = Z';

save('pulse.y', 'Z','-ascii');
