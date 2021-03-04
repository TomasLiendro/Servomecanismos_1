data1 = load('sin_0001.txt');
data2 = load('sin_0002.txt');
data3 = load('sin_0005.txt');
data4 = load('sin_001.txt');
data5 = load('sin_002.txt');
data6 = load('sin_005.txt');
data7 = load('sin_01.txt');
dt=[0.001 0.002 0.005 0.01 0.02 0.05 0.1];
    
duration=[mean(data1(:,3)) mean(data2(:,3)) mean(data3(:,3)) mean(data4(:,3)) mean(data5(:,3)) mean(data6(:,3)) mean(data7(:,3))];
delay = dt-duration;
% plot(data(:,1), data(:,3), '*');
bar([1:7], [duration; dt], 'grouped')