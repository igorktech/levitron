


global g
g = 9.8
global L 
L = 0.00388
L1 = L
L2 = L
L3 = L
L4 = L
global R 
R = 4
R1 = R
R2 = R
R3 = R
R4 = R
u1 = 12
global i1 
i1 = u1/(2*R)
u2 = 12
global i2 
i2 = u2/(2*R)
global m 
m = 0.01
global l
l = 0.0567/2
y1 = 0
y2 = 0
z1 = 0.03
z2 = 0
global x3 
x3 = i
global x1 
x1 = 0
x2 = 0
x3 = 0

a = i1^2*z1/((l^2+z1^2)^(3/2))
b = i1^2*l/((l^2+z1^2)^(3/2))
global K 
K = m*g*z1*z1/i/i
global K1 
% K1 = m*g/((1+a)*b)
K1 = m*g/(2*b)
global K2 
% K2 = K1*a
K2 = K1
global K3 
K3 = K1
global K4 
K4 = K2


% x0 = [x1 ;0 ;x3]
%  eps = 0.001;
%  u1 = u:-0.2:0.2;
%  u = u1;
%   x0 = [x1; x2;x3];
%  xx = [];
%  for i = 1:length(u1)
%  x = newton('get_F7', 'get_G7', x0, u1(i), eps);
%  xx = [xx x];
%  x0 = x;
%  end
% plot(u(1,:), xx(1,:),'LineWidth', 2)
%  hold on
%  grid on
%  plot(u(1,:), xx(2,:),'k--')
%  plot(u(1,:), xx(3,:), '-*')
%  xlabel('U, в.')
%  ylabel('Ф, \omega, i_д, о.е.')
%  legend('x(U)','v(U)','i(U)');
% 
% 
% 
% 
% 
% function f = get_F7(x,u)
% global g
% global L 
% global R 
% global i 
% global m 
% global K 
% 
%     f = [x(2); g-K/m*(x(3)^2/x(1));u/L-R*x(3)/L]
% end
% 
% 
% 
% function gk = get_G7(x,u)
% global g
% global L 
% global R  
% global i 
% global m 
% global K 
%     gk = [0 1 0;K*x(3)^2/m/(x(1)^2) 0 -2*K*x(3)/m/(x(1)^2); 0 0 -R/L];
% end
% 
% function [x]=newton(F,G,x0,u,e)
%     y=feval(F,x0,u);
%     x=x0;
% while(norm(y)> e)
%     gr=feval(G,x,u);
%     x=x-inv(gr)*y;
%     y=feval(F,x,u);
%     clc,disp(y)
% end
% end