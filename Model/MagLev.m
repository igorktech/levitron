% 
%  clc;
% clear;

global g
g = 9.8
global L 
L = 0.00392
global R 
R = 10.8
u = 15
global i 
i = u/R
global m 
m = 0.03
global x3 
x3 = i
global x1 
x1 = -0.03
x2 = 0
global K 
K = m*g*x1*x1/i/i

eps = 500;
u1 = u:-0.2:0.2;
x0 = [-0.025; x2;x3];
xx = [];
for i = 1:length(u1)
    x = newton('get_F7', 'get_G7', x0, u1(i), eps);
    
    xx = [xx x];
    x0 = x;
end
% 
% figure(1)
% plot(u1(1,:), xx(1,:),'LineWidth', 2)
% hold on
% grid on
% plot(u1(1,:), xx(2,:),'k--')
% plot(u1(1,:), xx(3,:), '-*')
% xlabel('U, в.')
% ylabel('x(U), v(U), i(U)')
% legend('x(U)','v(U)','i(U)');
%     
    
    %linearization
%     
%     a = [0 1 0; 2*K*x3^2/m/x1^3 0 -2*K*x3/m/x1^2; 0 0 -R/L];
%     b = [0 ;0 ; 1/L];
%     c = [1 0 0];
%     d = 0;
%     P = poly(a)
%     XP = roots(P);
%     figure(2)
%     grid on; hold on;
%     plot(real(XP),imag(XP), '*');
%     t = 0:1e-4:20;
%     
%     sys = ss(a,b,c,d)
%     tf_sys = tf(sys)
%     figure(3)
%     rlocus(sys)
% 
%     
%     x0 = [-0.025;0;0];
%     u=[3.5*ones(size(t))];
%     figure(4)
%     [y,x] = LMSIM(a,b,c,d,t,x0,u);
%         figure(6)
%     grid on; hold on;
%     plot(t,x(1, :));
% 
%     u = zeros(length(t),1);
%     u(t>=0) = 3.5;
%     
%     [yyy,xxx] = lsim(tf_sys,u,t,x0);
%     figure(7)
%     plot(t,yyy(1, :));


    kp = -4.25; % с уменьшением возрастает перерегулирование
    ki = -0; % с уменьшением возрастает быстродействие и перерегулирование
    kd = -320; % с уменьшением возрастает быстродействие, но возрастает перергулирование
    u = 3.5;
    x10 = -0.04;
    num = xlsread('table.xlsx');
    ref = num(2:3,:)';
    ref(:,1) = num(3,:)';
    ref(:,2) = num(2,:)';
        ref2 = num(2:3,:)';
    ref2(:,1) = num(3,:)';
    ref2(:,2) = num(1,:)';
%     out = sim('lev.mdl');
%     %create tf from sim model
%     tfdata = iddata(out.inputdata.signals.values,out.outputdata.signals.values,0.001);
%     N = 5;
%     sys_sim = tfest(tfdata,N)
%     figure(8)
%     bodeplot(sys_sim)
%     figure(9)
%     nyquist(sys_sim)
%     figure(10)
%     step(sys_sim,t)
%     figure(11)
%     x0 = [-0.25;0;0;0;0];
%     lsim(sys_sim,u,t,x0)
%     figure(12)
%     rlocus(sys_sim)
%     figure(13)
%     hold on
%     grid on
%     Legend = []
% for k = 1:1.5:5
%     x10 = x10-0.01
%     kd = 0;
% for i=2:4
%     kd = kd - 30*i;
%     out = sim('lev.mdl');
%     if u>2.5
%     plot(out.outputdata.time,out.outputdata.signals.values,'--','LineWidth', 1)
%     else
%         plot(out.outputdata.time,out.outputdata.signals.values,'-.','LineWidth', 1)
%     end
% 
%     t = append("kp="  , num2str(kp),",ki=",num2str(ki),",kd=",num2str(kd))
%     t = append(t,",u=",num2str(u),",x10=",num2str(x10))
%     Legend = [Legend t];
%     legend(Legend);
% end
% end
% 
%   
    
    
    
    
    
    
    
    
    
    %functions
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
    
    function [y,x]=LMSIM(a,b,c,d,t,x0,u)
    h=t(2)-t(1);
    % matrix exponent
    [m,n] = size(a);
    [m,nb] = size(b);
    s = expm([[a b]*h; zeros(nb,n+nb)]);
    p = s(1:n,1:n);
    g= s(1:n,n+1:n+nb);
    %
    % time process
    %
    x=(ltitr(p,g,u',x0'))';
    y=c*x+d*u;
    end
