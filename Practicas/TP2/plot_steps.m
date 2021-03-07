clear all
close all
data = load('Esca.txt');
plot(data(:,1), data(:,2), '-*');
title('Función escalones random')
xlabel('Tiempo (s)')
ylabel('esc(T_1,T_2,V0)')
