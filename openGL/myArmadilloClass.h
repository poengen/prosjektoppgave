// myArmadilloClass.h
//C++ code for solving poisson 2D
#include <iostream>
#include <armadillo>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

using namespace std;
using namespace arma;

#define PI 3.14159
//Number of internal grid points n
int const n = 25; //maks 256 for GLshort


class MyArmadilloClass
{
	private:
	int k;
	double hx, ht, T;
	mat A, G, I, H;
	
	public:
	MyArmadilloClass();
	vec getDiffusion(vec u);
	
	double getDouble();
	mat getMat();
};
