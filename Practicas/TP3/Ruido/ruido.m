clear all
close all
% 
% data1 = load('0VCH0ruido0001.txt');
% data2 = load('0VCH0ruido001.txt');
% data3 = load('0VCH0ruido01.txt');
% data4 = load('0VCH1ruido0001.txt');
% data5 = load('0VCH1ruido001.txt');
% data6 = load('0VCH1ruido01.txt');

data1 = load('2VCH0ruido0001.txt');
data2 = load('2VCH0ruido001.txt');
data3 = load('2VCH0ruido01.txt');
data4 = load('2VCH1ruido0001.txt');
data5 = load('2VCH1ruido001.txt');
data6 = load('2VCH1ruido01.txt');

dt=[0.001 0.002 0.005 0.01 0.02 0.05 0.1];
figure
plot(data1(:,1), data1(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')

axis([0 max(data1(:,1)) 1.95  2.05 ]);
title('Medición del ruido CH0 para Dt=0.001')
figure
plot(data2(:,1), data2(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')
axis([0 max(data2(:,1)) 1.95  2.05 ]);
title('Medición del ruido CH0 para Dt=0.01')
figure
plot(data3(:,1), data3(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')
axis([0 max(data3(:,1)) 1.95  2.05 ]);
title('Medición del ruido CH0 para Dt=0.1')



figure
plot(data4(:,1), data4(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')
axis([0 max(data4(:,1)) 1.95  2.05 ]);
title('Medición del ruido CH1 para Dt=0.001')
figure
plot(data5(:,1), data5(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')
axis([0 max(data5(:,1)) 1.95  2.05 ]);
title('Medición del ruido CH1 para Dt=0.01')
figure
plot(data6(:,1), data6(:,2),"*")
grid on
ylabel('Voltaje (V)')
xlabel('Tiempo (s)')
axis([0 max(data6(:,1)) 1.95 2.05 ]);
title('Medición del ruido CH1 para Dt=0.1')