% Two dimensional steady state poisson solver

%% Defining variables
n = 100; %internal grid points
h = 1/(n+1); %intervals

%% The Grid
x = zeros(n+2,1);
for i = 2:n+2
    x(i) = (i-1)*h;
end

%% Analytical Solution
uHandle = @(x,y) sin(pi*x)*sin(pi*y);
uAnal = zeros(n+2,n+2);
for i = 1:n+2
    for j = 1:n+2
        uAnal(i,j) = uHandle(x(i),x(j));
    end
end

%% Load vector
fHandle = @(x,y) 2*pi^2*sin(pi*x)*sin(pi*y);
fMat = zeros(n+2,n+2);
for i = 1:n+2
    for j = 1:n+2
        fMat(i,j) = fHandle(x(i),x(j));
    end
end
fMat2 = fMat(2:n+1,2:n+1);
f = reshape(fMat2,n*n,1);

%% Matrices
e1 = ones(n,1);
Adiag = spdiags([-e1 4*e1 -e1], -1:1, n, n);
Asub = diag(-e1);
Asup = diag(-e1);

A = 1/h^2*blktridiag(Adiag,Asub,Asup,n);

%% Solve the linear system by LU-decomposition
u = A\f;

%% Composing the solution matrix Unum
Unum = reshape(u,n,n);
Usol = zeros(n+2,n+2);
Usol(2:n+1,2:n+1) = Unum;
mesh(x,x,Usol)

%% Plotting
figure(1);
subplot(1,2,1);
mesh(x,x,uAnal);
title('Analytical solution');
subplot(1,2,2);
mesh(x,x,Usol)
title('Numerical solution');