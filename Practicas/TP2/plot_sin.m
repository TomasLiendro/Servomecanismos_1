data = load('Seno.txt');
plot(data(:,1), data(:,2), '-*');
axis([0 2 0 4])
title('Función seno')
xlabel('Tiempo (s)')
ylabel('sin(w*t)')
