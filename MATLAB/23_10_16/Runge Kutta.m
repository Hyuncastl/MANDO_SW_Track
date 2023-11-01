clc;
clear all;
close all;
f = input('\n Input Function (dy/dx) = ', 's');
f = str2func(['@(x, y)' f]);  % Convert the input function to an anonymous function
x0 = input('\n Initial x0 = ');
y0 = input('\n Initial y0 = ');
h = input('\n Step size h = ');
xg = input('\n Given Value xg = ');

n = (xg - x0) / h;

x_values = zeros(1, n);
y_values = zeros(1, n);

disp(['itn x            k1      k2       k3      k4       k      ' ...
    '' ...
    'yg'])
for i = 1:n
    k1 = h * f(x0, y0);
    k2 = h * f(x0 + (h / 2), y0 + (k1 / 2));
    k3 = h * f(x0 + (h / 2), y0 + (k2 / 2));
    k4 = h * f(x0 + h, y0 + k3);
    k = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    yg = y0 + k;
    x0 = x0 + h;
    y0 = yg;

    x_values(i) = x0;
    y_values(i) = y0;

    fprintf('\n %d%.4f \t%.4f \t%.4f \t%.4f \t%.4f \t%.4f \t%.4f', i, x0, k1, k2, k3, k4, k, yg);
end

plot(x_values, y_values);
xlabel('x');
ylabel('y');
title('Solution of the Differential Equation');
grid on;
