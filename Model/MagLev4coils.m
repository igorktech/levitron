


global g
g = 9.8
global L 
L = 0.00388%/2 % чтобы не менять модель, потом переделать!
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
% 
x10z = -0.03
x10y = -0.008
x10x = -0.008

  Legend = []

    figure(1)
    hold on
    grid on
for i=1:3
   x10x = x10x + i*0.004;
    out = sim('lev4coils.mdl');
   
        plot(out.scopeOutz.time,out.scopeOutz.signals.values,'-.','LineWidth', 1)
        t = append("sim="  , num2str(i),",z0=",num2str(x10z));
        Legend = [Legend t];
        plot(out.scopeOutx.time,out.scopeOuty.signals.values,'-.','LineWidth', 1)
        t = append("sim="  , num2str(i),",y0=",num2str(x10y));
        Legend = [Legend t];
        plot(out.scopeOuty.time,out.scopeOutx.signals.values,'-.','LineWidth', 1)
        t = append("sim="  , num2str(i),",x0=",num2str(x10x));
        Legend = [Legend t];
    

    legend(Legend);
end

   
for i=1:3
   x10y = x10y + i*0.004;
    out = sim('lev4coils.mdl');
   
        plot(out.scopeOutz.time,out.scopeOutz.signals.values,'--','LineWidth', 1)
        t = append("sim="  , num2str(i+3),",z0=",num2str(x10z));
        Legend = [Legend t];
        plot(out.scopeOutx.time,out.scopeOuty.signals.values,'--','LineWidth', 1)
        t = append("sim="  , num2str(i+3),",y0=",num2str(x10y));
        Legend = [Legend t];
        plot(out.scopeOuty.time,out.scopeOutx.signals.values,'--','LineWidth', 1)
        t = append("sim="  , num2str(i+3),",x0=",num2str(x10x));
        Legend = [Legend t];
    

    legend(Legend);
end
    
%xyz
% 
% x10z = -0.03
% x10y = -0.008
% x10x = -0.008
% 
%   Legend = []
% 
%     figure(2)
%     hold on
%     grid on
% for i=1:3
%    x10x = x10x + i*0.004;
%     out = sim('lev4coils.mdl');
%    
%     plot(out.scopeOutx.signals.values,out.scopeOuty.signals.values)
%         
%         t = append("sim="  , num2str(i),",z0=",num2str(x10z));
%         
%        
%         t = append(t,",y0=",num2str(x10y));
%        
%         
%         t = append(t,",x0=",num2str(x10x));
%         Legend = [Legend t];
%     
% 
%     legend(Legend);
% end
% 
%    
% for i=1:3
%    x10y = x10y + i*0.004;
%     out = sim('lev4coils.mdl');
%    
%     plot(out.scopeOutx.signals.values,out.scopeOuty.signals.values,'--')
%         
%         t = append("sim="  , num2str(i),",z0=",num2str(x10z));
%         
%        
%         t = append(t,",y0=",num2str(x10y));
%        
%         
%         t = append(t,",x0=",num2str(x10x));
%         Legend = [Legend t];
%     
% 
%     legend(Legend);
% end
%  