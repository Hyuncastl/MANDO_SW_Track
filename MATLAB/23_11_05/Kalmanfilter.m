clear all;
clc;close all; 

A = [1.1269 -0.4940 0.1129, 
1.0000 0 0, 
0 1.0000 0]; 
B = [-0.3832 
0.5919 
0.5191]; 
C = [1 0 0]; 
D = 0; 
t = (0:100)'; 
u = 10*sin(t*0.01*pi);
Q = 1; 
R = 5;
w = sqrt(Q)*randn(length(t),1);
v = sqrt(R)*randn(length(t),1); 

sys = ss(A,B,C,D,-1); 
yt = lsim(sys,u); 
y = lsim(sys,u+w); 
yv = y + v;

P=B*Q*B';
x=zeros(3,1);
ye = zeros(length(t),1); 
ycov = zeros(length(t),1); 
errcov = zeros(length(t),1); 

for i=1:length(t) 
K = P*C'/(C*P*C'+R); 
x = x + K*(yv(i)-C*x);
P = (eye(3)-K*C)*P;  

ye(i) = C*x; 
errcov(i) = C*P*C'; 
x = A*x + B*u(i); 
P = A*P*A' + B*Q*B';
end 

subplot(211), 
plot(t,yt,'Color',[0.75 0.75 0.75],'linewidth',3) 
hold on 
plot(t,y,'b',t,ye,'r--'), 
xlabel('No. of samples'), ylabel('Output') 
legend('true value','signal with noise','siganl with Kalman filter') 
title('Response with time-varying Kalman filter') 
subplot(212), 
plot(t,yt-yv,'g',t,yt-ye,'r--'), 
xlabel('No. of samples'), ylabel('Error') 
legend('true value - signal with noise','true value - signal with Kalman filter') 
