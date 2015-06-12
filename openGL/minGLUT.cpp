#include "myArmadilloClass.h"
#include <sys/time.h>

//Vertices, indices and color buffers
GLdouble vertices[3*n*n]; //3*n^2		n=3(27), n=4(48) //x,y,z coordinates of vertices
GLshort indices[6*(n-1)*(n-1)]; //6*(n-1)^2	n=3(24), n=4(54) //index numbering
GLubyte colors[3*n*n]; //RGB, color coordinates for each vertex

//Armadillo variables
vec u = zeros<vec>(n*n); //numerical solution vector of size n*n
MyArmadilloClass myArmadilloStuff; //contructs a variable of type MyArmadilloClass

struct timeval lastTime;
//double angle = 0.0f;
void reshape(int w, int h);
void display(void);
void processNormalKeys(unsigned char key, int x, int y);

//mouse input
void mouseClick(int button, int state, int x, int y);
void mouseDrag(int x, int y);
void mouseMove(int x, int y);
void mouseAction(int x, int y);

void minGRID();
void myDriver();
void updateU();
void updateVertices();

void print1DArray(int size, GLshort array[]);

int main(int argc, char** argv)
{
	minGRID(); //initialize the buffers vertices, colors and indices to create the grid

	//print1DArray(6*(n-1)*(n-1),indices);

    	glutInit(&argc, argv); //initialize GLUT
	glutInitWindowPosition(300,300); //window position, pixels from top left corner	
    	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); //display mode
    	glutInitWindowSize(1000, 1000); //window size, width, height in pixels (640,480)
    	glutCreateWindow("Quack Program"); //create window

    	glutDisplayFunc(display); //requires the window to be painted
    	//glutReshapeFunc(reshape);
    	
	//glutIdleFunc(display); // keep calling display (rendering function) when idle
	glutIdleFunc(myDriver); //does the numerical simulation, this is where the program operates mostly
	glutKeyboardFunc(processNormalKeys);

	// mouse input
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseDrag);
	glutPassiveMotionFunc(mouseMove);

    	glutMainLoop(); //event processing loop

    	return EXIT_SUCCESS;
}

void print1DArray(int size, GLshort array[]){
		for (int i = 0; i < size; i++){
			cout << array[i] << "\t";
		}
		cout << endl;
}

void mouseClick(int button, int state, int x, int y) {
    //cout << "Mouse click at " << x << ", " << y << " button " << button << "  state " << state << endl;

}
void mouseDrag(int x, int y) {
     	//cout << "Mouse drag at " << x << ", " << y << endl;
	mouseAction(x,y);
	
}
void mouseMove(int x, int y) {
     //cout << "Mouse move at " << x << ", " << y << endl;


}

void mouseAction(int x, int y){
	int pph = 1000/(n-1); //pixels per interval
	float closestIndexX = (float)x/(float)pph;
	float closestIndexY = (float)y/(float)pph;
	int ix = round(closestIndexX); int iy = (n-1)-round(closestIndexY);
	vertices[3*n*ix+3*iy+2] = 1; //z-coordinate
	colors[3*ix+3*n*iy] = 255; //red color
	//colors[3*n*ix+3*iy+2] = 255; //blue color
	//cout << "Mouse move at: (" << x << ", " << y << "). Index is: (" << ix << ", " << iy << ")." << endl;
}

long getTimePassed() {
	struct timeval time_now;
	long seconds, useconds;
	gettimeofday(&time_now, NULL);
	seconds  = time_now.tv_sec  - lastTime.tv_sec;
	useconds = time_now.tv_usec - lastTime.tv_usec;
	lastTime = time_now;
	return ((seconds*1000) + useconds/1000.0);
}

void myDriver() {
	
    // figure out how long since last time
	long t = getTimePassed();
	updateU(); //copy z-coordinate of vertices into the solution vector u
	u = myArmadilloStuff.getDiffusion(u); //numerical algorithm for diffusion (one time step)
	updateVertices(); //updating z-components of vertices and colors for plotting

	t = getTimePassed();
	//cout << "n=25: Time in myDriver() = " << t << endl;
	glutPostRedisplay();
}

void updateU(){
    int k=0;
    for(int i=0; i<n; i++) {
	for(int j=0; j<n; j++) {
	    u(k++) = (double) vertices[3*n*i+3*j+2];
	}
    }	
}

void updateVertices() {
    int k=0;
    GLdouble temp_z;
    for(int i=0; i<n; i++) {
	for(int j=0; j<n; j++) {
	    	temp_z = (GLdouble) u(k++);
		vertices[3*n*i+3*j+2] = temp_z; //z-coordinate

		colors[3*i+3*n*j] = 255*temp_z; //red color (0-255) strongest when z=0
		colors[3*n*i+3*j+1] = 0; //green color (0-255) strongest when z=0.5
		colors[3*n*i+3*j+2] = 0; //blue color (0-255) strongest when z=1

		//colors[3*i+3*n*j] = 200*(1-temp_z)+55; //red color
		//colors[3*n*i+3*j+1] = 800*(1-temp_z)*temp_z+55; //green color
		//colors[3*n*i+3*j+2] = 200*temp_z+55; //blue color
	}
    }
}

void display(void)
{
	glClearColor(0,0,0,.5); //set background colour to black
	glEnable(GL_DEPTH_TEST); //enable depth
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); // reset transformations
	
/*
	gluLookAt(	0.5, 0.5, -1.0, //position of eye point (origo)
			0.5, 0.5, 0.0, //position of reference point (where to look)
			0.0, 0.0, 1.0); //direction of up vector
*/	
	//glRotatef(angle,0.0,0.0,1.0);
	//glScalef(1,1,-1);
	//glTranslatef(1.0,1.0,0.0);
	//glLoadIdentity(); // reset transformations

	glColor3f(0,1,1);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();

	glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);
	glVertexPointer(3, GL_DOUBLE, 0, vertices);
	glDrawElements(GL_TRIANGLES, 6*(n-1)*(n-1), GL_UNSIGNED_SHORT, indices);

	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//angle+=0.5;
    	glutSwapBuffers();
}

void minGRID() {
	double h = 1./(double)(n-1);
	double x[n];
	
	for (int i=0; i<n; i++){
		x[i] = -1+i*2*h; //= i*h; (shift quadrant)	
	}

	double temp_z;
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			vertices[3*i+3*n*j] = x[i]; //x-coordinate
			vertices[3*n*i+3*j+1] = x[i]; //y-coordinate
			temp_z = cos(0.5*PI*x[i])*cos(0.5*PI*x[j]); // sin(PI*x[i])*sin(PI*x[j]);
			vertices[3*n*i+3*j+2] = temp_z; //z-coordinate

			colors[3*i+3*n*j] = 255*temp_z; //red color (0-255) strongest when z=0
			colors[3*n*i+3*j+1] = 0; //green color (0-255) strongest when z=0.5
			colors[3*n*i+3*j+2] = 0; //blue color (0-255) strongest when z=1



			//colors[3*i+3*n*j] = 200*(1-temp_z)+55; //red color (0-255) strongest when z=0
			//colors[3*n*i+3*j+1] = 800*(1-temp_z)*temp_z+55; //green color (0-255) strongest when z=0.5
			//colors[3*n*i+3*j+2] = 200*temp_z+55; //blue color (0-255) strongest when z=1
		}
	}

	int index;
	int a;
	for (int k=0; k<2; k++){
		index = 0;
		for (int l=0; l<n-1; l++){
			index = l*n;
			for (int j=0; j<3; j++){
				if (k==0 && j==1){ index = index - (n-2); }

				else if (k==0 && j==2){ index++; }
				else if (k==1 && j==1){ index++; }
				else if (k==1 && j==2){ index = index - (n-2); }
				
				for(int i=0; i<n-1; i++){
					a = 3*k*(n-1)*(n-1) + 3*l*(n-1) + j + 3*i;
					indices[a] = index; //this loop creates the index buffer
					index++;
				}
			}
		}
	}

// TAKES AWAY UPPER OR LOWER TRIANGLES
//for (int i=0; i<(6*(n-1)*(n-1))/2; i++){indices[i]=0;}
//for (int i=(6*(n-1)*(n-1))/2; i<6*(n-1)*(n-1); i++){indices[i]=0;}
// PRINT FUNCTION
//for (int i=0; i<6*(n-1)*(n-1); i++){cout << indices[i] << endl;} //print indices buffer
//for (int i=0; i<3*n*n; i++){cout << vertices[i] << endl;} //print vertices buffer
	
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 27) //press ESC to exit app
		exit(0);
}

void reshape(int w, int h) //NESCESSARY IF WINDOW NOT CHANGED?
{
    	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION); // Use the Projection Matrix
	glLoadIdentity(); // Reset Matrix
	glViewport(0, 0, w, h); // Set the viewport to be the entire window

	// Set the correct perspective.
	// gluPerspective(45,ratio,1,100); //last two defines near and far clipping plane

	glMatrixMode(GL_MODELVIEW); // Get Back to the Modelview
}
