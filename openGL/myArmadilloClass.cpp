#include "myArmadilloClass.h"

MyArmadilloClass::MyArmadilloClass(){ //constructor
	hx = 1./double(n-1);
	ht = 0.001; //time-step
	k = 100; //number of time-steps
	T = ht*k; //total time

	A = zeros<mat>(n*n,n*n);
	I = zeros<mat>(n*n,n*n);
	G = zeros<mat>(n*n,n*n);
	H = zeros<mat>(n*n,n*n); //advection matrix
	for (int i = 0; i < n*n; i++){ A(i,i) = 4; I(i,i) = 1;}
	for (int i = 0; i < n*n-n; i++) {
		A(i,n+i) = -1; A(n+i,i) = -1;
		H(i,n+i) = 0.5;  H(n+i,i) = -0.5;
	}
	for (int i = 0; i < n*n-1; i++) {
		A(i,i+1) = -1; A(i+1,i) = -1;
		H(i,i+1) = 0.5;  H(i+1,i) = -0.5;
	}
	for (int i = 0; i < n-1; i++) {
		A((i+1)*n-1,(i+1)*n) = 0; A((i+1)*n,(i+1)*n-1) = 0;
		H((i+1)*n-1,(i+1)*n) = 0; H((i+1)*n,(i+1)*n-1) = 0;
	}
	H = -5/(2*hx)*H;
	A = -0.1/(hx*hx)*A;
	A = A+H;
	G = I-ht*A;
}

double MyArmadilloClass::getDouble(){
	return hx;
}

mat MyArmadilloClass::getMat(){
	return G;
}

vec MyArmadilloClass::getDiffusion(vec u) {
	u = solve(G,u);
	return u;
}
