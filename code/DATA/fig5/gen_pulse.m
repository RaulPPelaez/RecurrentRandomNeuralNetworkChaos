




x = 0:(1000+300);
bell = normpdf(x, 1000, 30);

Z = 0.2+(  (1-0.2)/max(bell)   )*bell;
Z = Z';

save('pulse.y', 'Z','-ascii');
