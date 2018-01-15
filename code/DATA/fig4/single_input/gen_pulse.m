




x = 0:N;
bell = normpdf(x, x0, 30);

Z = 0.2+(  (1-0.2)/max(bell)   )*bell;
Z = Z';

save('pulse.y', 'Z','-ascii');
