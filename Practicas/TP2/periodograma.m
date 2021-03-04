%% Calculo de Periodograma para estimacion de Densidad Espectral de Potencia
close all
clear all
clc
%parametros del experimento
%Período de muestreo en segundos
Dt = 0.01;
% Frecuencia de muestreo en Hz
Fs = 1/Dt;
% Numero de muestras potencia de 2
Ns = 2^10;
% Tiempo inicial y final
T0 = 0;
Tf = Ns*Dt;

% Para demostracion utilizo una señal ficticia con ruido agregado
rng default
% Arreglo de tiempos
t = 0:Dt:(Ns-1)*Dt;
% Importo la funcion
% data = load('Seno_1024_001.txt');
data = load('Chirp_1024_001.txt');
% data = load('RndSteps_1024_001.txt');

x = data(:,2);
% Arreglo de funcion
ruido = 0.01;

% x = 10* cos(2*pi*10*t) + 20 * cos(2*pi*20*t)
xr = x + ruido * rand(size(t));

%% Calculo del periodograma
% Los alumnos deben entender y poder explicar los calculos que se realizan aquí
N = length(x);
xdft = fft(x); % Calculo la Transformada de Fourier
xdft = xdft(1:N/2+1); % Me quedo con la primera mitad de los datos mas 1 
                      % porque la T. de Fourier esta espejada respecto a la
                      % mitad del espectro.
psdx = (1/(Fs*N)) * abs(xdft).^2; % Calcula la potencia espectral, 1/Fs=Dt,
                                  
psdx(2:end-1) = 2*psdx(2:end-1); % Como se toma la mitad del dominio, se
                      % multiplica por 2 para conservar la energia total
freq = 0:Fs/length(x):Fs/2; % Vector de frecuencias desde 0 hasta la Frec de
                      % Nyquist. La frecuencia mas chica es Fs/N.

%graficamos la señal y el periodograma
% plot(t,x,'*-');
% hold on
plot(t, xr);
title('Señal Ficticia')
xlabel('Tiempo [seg]')
ylabel('Señal [??]')
figure
plot(freq,10*log10(psdx))
grid on
title('Periodograma Usando FFT')
xlabel('Frequencia (Hz)')
ylabel('Potencia/Frequencia (dB/Hz)')