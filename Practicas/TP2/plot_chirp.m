clear all
close all
data = load('Chirp.txt');
plot(data(:,1), data(:,2), '-*');
% axis([0 2 0 4])
title('Función chirp')
xlabel('Tiempo (s)')
ylabel('chirp(w_0,w_1,C)')
