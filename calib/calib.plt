reset
set terminal qt
f(x)= a*x+b
fit f(x) 'calib.dat' u 1:2 via a,b
plot 'calib.dat' using 1:2,\
f(x)