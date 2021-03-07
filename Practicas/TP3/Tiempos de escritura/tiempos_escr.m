clear all
close all

data1 = load('con_0001.txt');
data2 = load('con_0002.txt');
data3 = load('con_0005.txt');
data4 = load('con_001.txt');
data5 = load('con_002.txt');
data6 = load('con_005.txt');
data7 = load('con_01.txt');

dt=[0.001 0.002 0.005 0.01 0.02 0.05 0.1];

figure
plot(data1(:,1), dt(1)-data1(:,3))
hold on
grid on
h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.001')
figure
plot(data2(:,1), dt(2)-data2(:,3))
hold on
grid on
h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.002')
figure
plot(data3(:,1), dt(3)-data3(:,3))
hold on
grid on
h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.005')


figure
plot(data4(:,1), dt(4)-data4(:,3))
hold on
grid on

h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.01')

figure
plot(data5(:,1), dt(5)-data5(:,3))
hold on
grid on

h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.02')
figure
plot(data6(:,1), dt(6)-data6(:,3))
hold on
grid on

h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.05')
figure
plot(data7(:,1), dt(7)-data7(:,3))

h = legend('Tiempo de lectura');
h.Location = 'southeast'
ylabel('Tiempo de lectura (s)')
xlabel('Tiempo (s)')
title('Tiempo de lectura para Dt=0.1')
grid on
