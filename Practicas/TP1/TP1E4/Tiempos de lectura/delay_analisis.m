% clear all
% close all
% 
% data1 = load('lect_C_0001.txt');
% data2 = load('lect_C_0002.txt');
% data3 = load('lect_C_0005.txt');
% data4 = load('lect_C_001.txt');
% data5 = load('lect_C_002.txt');
% data6 = load('lect_C_005.txt');
% data7 = load('lect_C_01.txt');
% % 
% 
% 
% dt=[0.001 0.002 0.005 0.01 0.02 0.05 0.1];
%     
% duration=[mean(data1(:,3)) mean(data2(:,3)) mean(data3(:,3)) mean(data4(:,3)) mean(data5(:,3)) mean(data6(:,3)) mean(data7(:,3))];
% delay = dt-duration;
% 
% plot(dt, duration,'*')
% bar([1:7], [duration; dt], 'grouped')

clear all
close all

data1 = load('sin_0001.txt');
data2 = load('sin_0002.txt');
data3 = load('sin_0005.txt');
data4 = load('sin_001.txt');
data5 = load('sin_002.txt');
data6 = load('sin_005.txt');
data7 = load('sin_01.txt');

dt=[0.001 0.002 0.005 0.01 0.02 0.05 0.1];
figure
plot(data1(:,1), data1(:,3))
hold on
plot([0 max(data1(:,1))], [dt(1), dt(1)])
grid on
axis([0 max(data1(:,1)) min(data1(:,3)) dt(1)+dt(1)/10])
h = legend('Tiempo de espera', 'Dt');
h.Location = 'southeast'
ylabel('Tiempo de espera (s)')
xlabel('Tiempo')
title('Tiempo de espera para Dt=0.001')


figure
plot(data4(:,1), data4(:,3))
hold on
plot([0 max(data4(:,1))], [dt(4), dt(4)])
grid on
axis([0 max(data4(:,1)) min(data4(:,3)) dt(4)+dt(4)/100])
h = legend('Tiempo de espera', 'Dt');
h.Location = 'southeast'
ylabel('Tiempo de espera (s)')
xlabel('Tiempo')
title('Tiempo de espera para Dt=0.01')



figure
plot(data7(:,1), data7(:,3))
hold on
plot([0 max(data7(:,1))], [dt(7), dt(7)])
grid on
axis([0 max(data7(:,1)) min(data7(:,3)) dt(7)+dt(7)/1000])
h = legend('Tiempo de espera', 'Dt');
h.Location = 'southeast'
ylabel('Tiempo de espera (s)')
xlabel('Tiempo')
title('Tiempo de espera para Dt=0.1')
