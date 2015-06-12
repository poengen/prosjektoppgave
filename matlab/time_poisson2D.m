% Solving the time dependent 2D poisson problem with given boundary
% conditions

%% Image initial condition
PIC2 = imread('init_pic2.jpg');
PIC = imread('init_pic2.jpg');
PIC = rgb2gray(PIC);
PIC = double(PIC);
PIC = 1/256*PIC;
n = length(PIC);
PIC = ones(n,n)-PIC;

%% Defining variables
%n = 30; %internal grid points
hx = 1/(n+1); %intervals

ht = 0.001; %time step
k = 100; %number of time steps
T = ht*k; %total time

%% The Grid
x = zeros(n+2,1); %includes the boundary
for i = 2:n+2
    x(i) = (i-1)*hx;
end

%% Matrices
I = eye(n^2);
e1 = ones(n,1);
Adiag = spdiags([-e1 4*e1 -e1], -1:1, n, n);
Asub = diag(-e1);
Asup = diag(-e1);

A = -1/hx^2*blktridiag(Adiag,Asub,Asup,n);
G = I-ht*A;

%% Initial conditions
p = zeros(n*n,k); %solution vector
p(:,1) = reshape(PIC,n*n,1); %initial condition

%% Apply boundary conditions

pSol = zeros(n+2,n+2,k);

%boundary functions
%f1 = @(x,t) 0.5*sin(t); %y=0
%f2 = @(y,t) 0.5*(1-y)*sin(t); %x=1
%f3 = @(x,t) (1-x)*sin(t); %y=1
%f4 = @(y,t) 0.5*(1+y)*sin(t); %x=0

%% Solve the linear system Au=f

for i = 1:k-1  
    p(:,i+1) = G\p(:,i); %G\(p(:,i)+ht/hx^2*f); 
end

%% Composing the solution matrix Usol
for i = 1:k
    pSol(2:n+1,2:n+1,i) = reshape(p(:,i),n,n);
end
%% Plotting

for i=1:k
    mesh(x,x,pSol(:,:,i));
    xlabel('x');
    ylabel('y');
    axis([0 1 0 1 -0.6 1]);
    pause(0.5);
end

%% rotate

B = imrotate(PIC2,-90);
%% Making figures


figure(1);
subplot(2,3,1)
image(B);

subplot(2,3,2)
surf(x,x,pSol(:,:,1));
    axis([0 1 0 1 -0.6 1]);
    
subplot(2,3,3)
surf(x,x,pSol(:,:,2));
    axis([0 1 0 1 -0.6 1]);
    
subplot(2,3,4)
surf(x,x,pSol(:,:,5));
    axis([0 1 0 1 -0.6 1]);
    
subplot(2,3,5)
surf(x,x,pSol(:,:,10));
    axis([0 1 0 1 -0.6 1]);
    
subplot(2,3,6)
surf(x,x,pSol(:,:,100));
    axis([0 1 0 1 -0.6 1]);

