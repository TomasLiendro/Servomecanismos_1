close all
clear all
dcg =[0 0 0 0 0 0];
data = load('escalon_1V.txt');
% plot(data(100:end-30,1), data(100:end-30,3), '-*');
% hold on
% plot(data(100:end-30,1), data(100:end-30,2), '-*');
offset=mean(data(100:200,3)-data(100:200,2))
figure()
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
dcg(5) = mean(data(900:1000,3)-offset-2);
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');
hold on
data = load('escalon_0.5.txt');
dcg(4) = mean(data(900:1000,3)-offset-2);
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');
data = load('escalon_1.5.txt');
dcg(6) = mean(data(900:1000,3)-offset-2);
hold on
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');

data = load('escalonm1.txt');
dcg(2) = mean(data(900:1000,3)-offset-2);
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');
hold on
data = load('escalon_m0.5.txt');
dcg(3) = mean(data(900:1000,3)-offset-2);
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');
data = load('escalon_m1.5.txt');
dcg(1) = mean(data(900:1000,3)-offset-2);
hold on
plot(data(100:end-30,1), data(100:end-30,3)-offset-2, '-*');
hold on
plot(data(100:end-30,1), data(100:end-30,2)-2, '-*');

figure
plot([-1.5 -1 -0.5 0.5 1 1.5], dcg,'-*')



figure(5)
data = load('escalon_1V.txt');
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
hold on
data = load('escalon_0.5.txt');
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
data = load('escalon_1.5.txt');
hold on
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
hold on
data = load('escalonm1.txt');
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
data = load('escalon_m0.5.txt');
hold on
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
data = load('escalon_m1.5.txt');
hold on
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');

figure(6)
data = load('escalon_1V.txt');
plot(data(100:end-30,1), (data(100:end-30,3)-offset-2)/(data(100:end-30,2)-2), '-');
hold on

figure
s = tf('s');

eps= sqrt((log(1.6)/pi)^2/((log(1.6)/pi)^2+1));
eps = 0.148;
wn = 32;
G = wn^2/(s^2+2*eps*wn*s+wn^2);
dato1V = load('escalon_1V.txt');
t = dato1V(501:end-100,1)-dato1V(501);
y = dato1V(501:end-100,3)-offset-2;
u = dato1V(501:end-100,2)-offset-2;
plot(t,y,'r')
hold on 
step(G)
legend('Planta', 'Simulacion')


% axis([0 2 0 4])
title('Función seno')
xlabel('Tiempo (s)')
ylabel('sin(w*t)')
