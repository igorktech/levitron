


global g
g = 9.8
global L 
L = 0.00392
global R 
R = 10.8
u = 12
global i 
i = u/R
global m 
m = 0.03
global x3 
x3 = i
global x1 
x1 = 0.03
x2 = 0
global K 
K = m*g*x1*x1/i/i

x0 = [x1 ;0 ;x3]
 eps = 0.001;
 u1 = u:-0.2:0.2;
 u = u1;
  x0 = [x1; x2;x3];
 xx = [];
 for i = 1:length(u1)
 x = newton('get_F7', 'get_G7', x0, u1(i), eps);
 xx = [xx x];
 x0 = x;
 end
plot(u(1,:), xx(1,:),'LineWidth', 2)
 hold on
 grid on
 plot(u(1,:), xx(2,:),'k--')
 plot(u(1,:), xx(3,:), '-*')
 xlabel('U, в.')
 ylabel('Ф, \omega, i_д, о.е.')
 legend('x(U)','v(U)','i(U)');





function f = get_F7(x,u)
global g
global L 
global R 
global i 
global m 
global K 

    f = [x(2); g-K/m*(x(3)^2/x(1));u/L-R*x(3)/L]
end



function gk = get_G7(x,u)
global g
global L 
global R  
global i 
global m 
global K 
    gk = [0 1 0;K*x(3)^2/m/(x(1)^2) 0 -2*K*x(3)/m/(x(1)^2); 0 0 -R/L];
end

function [x]=newton(F,G,x0,u,e)
    y=feval(F,x0,u);
    x=x0;
while(norm(y)> e)
    gr=feval(G,x,u);
    x=x-inv(gr)*y;
    y=feval(F,x,u);
    clc,disp(y)
end
end