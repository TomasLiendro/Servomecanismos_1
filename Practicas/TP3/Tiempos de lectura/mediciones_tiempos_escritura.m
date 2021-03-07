close all
con_01 = load('lect_C_01.txt');
con_001 = load('lect_C_001.txt');
con_0001 = load('lect_C_0001.txt');
con_002 = load('lect_C_002.txt');
con_0002 = load('lect_C_0002.txt');
con_0005 = load('lect_C_0005.txt');

%Mean delay time
mean_01 = mean(con_01(:,3));
mean_001 = mean(con_001(:,3));
mean_0001 = mean(con_0001(:,3));
mean_002 = mean(con_002(:,3));
mean_0002 = mean(con_0002(:,3));
mean_0005 = mean(con_0005(:,3));

s_01 = load('lect_S_01.txt');
s_001 = load('lect_S_001.txt');
s_0001 = load('lect_S_0001.txt');
s_002 = load('lect_S_002.txt');
s_0002 = load('lect_S_0002.txt');
s_0005 = load('lect_S_0005.txt');

%Mean delay time
s_mean_01 = mean(s_01(:,3));
s_mean_001 = mean(s_001(:,3));
s_mean_0001 = mean(s_0001(:,3));
s_mean_002 = mean(s_002(:,3));
s_mean_0002 = mean(s_0002(:,3));
s_mean_0005 = mean(s_0005(:,3));
con = [mean_0001, mean_0002, mean_0005, mean_001, mean_002, mean_01];
sin = [s_mean_0001, s_mean_0002, s_mean_0005, s_mean_001, s_mean_002, s_mean_01];

DT = [0.001, 0.002, 0.005, 0.01, 0.02, 0.1];
figure(1)
plot(DT, sin-con,'-*');
hold on;
grid on;
title('Medicion del tiempo de escritura');
xlabel('Dt [s]');
ylabel('Writing time [s]');
figure(2)
plot(DT, con, '-*', DT, sin ,'-*')
grid on;
title('Medicion del tiempo del loop');
legend('con', 'sin')
xlabel('Dt [s]');
ylabel('Tiempo de espera[s]');