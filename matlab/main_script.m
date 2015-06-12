



%% Two dimensional steady state poisson problem
% with homogeneous dirichlet boundary
n = 100; %internal grid points
uHandle = @(x,y) sin(pi*x)*sin(pi*y); %analytical solution
fHandle = @(x,y) 2*pi^2*sin(pi*x)*sin(pi*y); %load vector
g1=@(x) 0; g2=@(y) 0; g3=@(x) 0; g4=@(y) 0;


%% Run calculations
[x, uNum, uAnal, eH1, eL2, eMAX] = poisson2D_steady(n, uHandle, fHandle, g1, g2, g3, g4);

%% Error calculations

% = 100:(doble hver gang):10000 %(minst fem punkter)

n = [5,10,20,40,80,160,320];
%error_H1 = zeros(1,length(n));
error_L2 = zeros(1,length(n));
%error_MAX = zeros(1,length(n));
for i = 1:length(n)
    
    [x, uNum, uAnal, eH1, eL2, eMAX] = poisson2D_steady(n(i), uHandle, fHandle, g1, g2, g3, g4);
    %error_H1(i) = eH1;
    error_L2(i) = eL2;
    %error_MAX(i) = eMAX;
    
end

%% Error plot (ONLY LERROR)
figure(1);
he = 1./(n+1);
plot(he,error_L2,'b');
legend('Relative L2-error')
xlabel('h: grid space');
ylabel('error');

%% Loglog-plot of the error (ONLY LERROR)
figure(2);
he = 1./(n+1);
x2 = log(he(length(n))); x1 = log(he(1));

L2_2 = log(error_L2(length(n))); L2_1 = log(error_L2(1));
kL2 = (L2_2-L2_1)/(x2-x1); % slope for the L2-norm

loglog(he,error_L2,'-*b')
grid on
legend(['Relative L2-error, k = ',num2str(kL2)])
xlabel('Grid space h, log-scale');
ylabel('Relative L2-error, log-scale');

%% Plotting
figure(3);
subplot(1,2,1);
mesh(x,x,uAnal);
title('Analytical solution');
xlabel('x-axis');
ylabel('y-axis');
subplot(1,2,2);
mesh(x,x,uNum)
title('Numerical solution');
xlabel('x-axis');
ylabel('y-axis');