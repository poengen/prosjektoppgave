%% Two dimensional steady state poisson solver

function [x, uNum, uAnal, error_H1, error_L2, error_MAX] = poisson2D_steady(n, uHandle, fHandle, g1, g2, g3, g4)


%% Defining variables
h = 1/(n+1); %intervals

%% The Grid
x = zeros(n+2,1);
for i = 2:n+2
    x(i) = (i-1)*h;
end

%% Analytical Solution
uAnal = zeros(n+2,n+2);
for i = 1:n+2
    for j = 1:n+2
        uAnal(i,j) = uHandle(x(i),x(j));
    end
end

%% Load vector
fMat = zeros(n+2,n+2);
for i = 1:n+2
    for j = 1:n+2
        fMat(i,j) = fHandle(x(i),x(j));
    end
end
fMat2 = fMat(2:n+1,2:n+1);
f = reshape(fMat2,n*n,1);

uNum = zeros(n+2,n+2);
for j = 1:n %this loop adds the boundary values to the load vector
    f(j) = f(j) + 1/h^2*g1(x(j+1));
    f(n^2-n+j) = f(n^2-n+j) + 1/h^2*g3(x(j+1));
    f(n*j) = f(n*j) + 1/h^2*g2(x(j+1));
    f(n*j-n+1) = f(n*j-n+1) + 1/h^2*g4(x(j+1));
    
    %plotting only
    uNum(j+1,1) = g1(x(j+1));
    uNum(n+2,j+1) = g2(x(j+1));
    uNum(j+1,n+2) = g3(x(j+1));
    uNum(1,j+1) = g4(x(j+1));    
end
    %corners (plotting only)
    uNum(1,1) = g1(0);
    uNum(n+2,1) = g1(1);
    uNum(n+2,n+2) = g3(1);
    uNum(1,n+2) = g3(0);


%% Matrices
e1 = ones(n,1);
Adiag = spdiags([-e1 4*e1 -e1], -1:1, n, n);
Asub = diag(-e1);
Asup = diag(-e1);

A = 1/h^2*blktridiag(Adiag,Asub,Asup,n);

%% Solve the linear system
u = A\f;

%% Composing the solution matrix uNum

uNum(2:n+1,2:n+1) = reshape(u,n,n);

%% H1-Error
%{
[DNX, DNY] = gradient(uNum);
[DAX, DAY] = gradient(uAnal);

Xdiff = reshape(DNX,(n+2)*(n+2),1)-reshape(DAX,(n+2)*(n+2),1);
Ydiff = reshape(DNY,(n+2)*(n+2),1)-reshape(DAY,(n+2)*(n+2),1);

%U = reshape(uAnal(2:n+1,2:n+1),n*n,1);   %Analytical Solution
a = norm(Xdiff,2);%/norm(U);
b = norm(Ydiff,2);%/norm(U);
error_H1 = sqrt(a^2+b^2);
%error_H1 = sqrt(transpose(E)*A*E); %alternativ h1 error
%}
error_H1 = 0;

%% L2-Error
U = reshape(uAnal(2:n+1,2:n+1),n*n,1);   %Analytical Solution
LTE = A*U-f;
E = -A\LTE;
error_L2 = h*norm(E,2);
error_L2 = error_L2/(h*norm(U,2)); %relative error

%% MAX-Error
%error_MAX = max(abs(E));
error_MAX = 0;

end