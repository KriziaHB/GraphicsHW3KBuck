//Krizia Houston Buck 
//CSCI 6350 Graphics HW 4 ( HW 3 updated )
//Due: 03/7/2017 

//Video on setting up OpenGL: https://www.youtube.com/watch?v=8p76pJsUP44
//Tutorial for rotating cubes: https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html

/*
* OGL02Animation.cpp: 3D Shapes with animation
*/
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream> 
using namespace std; 

/* Global variables */
char title[] = "3 Cubes Rotating on an Axis with Camera Movement ";
GLfloat angleCube1 = 0.0f;  // Rotational angle for cube [KHB] 
GLfloat angleCube2 = 0.0f;  // Rotational angle for cube [KHB] 
GLfloat angleCube3 = 0.0f;  // Rotational angle for cube [KHB] 
char cube1axis = 'x'; // Cube 1 user axis choice [KHB] 
char cube2axis = 'y'; // Cube 2 user axis choice [KHB]
char cube3axis = 'z'; // Cube 3 user axis choice [KHB]
GLdouble xp, yp, zp, xd, yd, zd, xu, yu, zu; // for camera rotation [KHB] 
float theta = 0.0; 
int refreshMills = 15;        // refresh interval in milliseconds 
GLfloat mat_amb[] = { 0.2, 0.4, 0.8, 1.0 }; //[KHB] ambient 
GLfloat mat_dif[] = { 0.1, 0.3, 0.8, 1.0 }; //[KHB] diffuse 



							  /* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb); //[KHB]
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif); //[KHB] 


	// CHANGES FOR CAMERA BUT DOESN"T WORK  
	xp = 10*cos(theta); // ray is the distance from the origin and theta is the current angle [KHB]
	yp = 0; 
	zp = 10*sin(theta); 
	xd = -cos(theta); 
	yd = 0; 
	zd = -sin(theta); 
	xu = 0; 
	yu = 1; 
	zu = 0; 
	gluLookAt(xp, yp, zp, xd, yd, zd, xu, yu, zu); 
	theta += 10; 
	// ENDS HERE 



	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(-3.0f, 0.0f, -10.0f);  // Left most cube [KHB]

	//Take user input axis [KHB] 
	if (cube1axis == 'x')
		glRotatef(angleCube1, 1.0f, 0.0f, 0.0f);  // Rotate about (1,0,0) x-axis [KHB]
	else if (cube1axis == 'y')
		glRotatef(angleCube1, 0.0f, 1.0f, 0.0f);  // Rotate about (0,1,0) y-axis [KHB] 
	else if (cube1axis == 'z')
		glRotatef(angleCube1, 0.0f, 0.0f, 1.0f);  // Rotate about (0,0,1) z-axis [KHB]
	else
		glRotatef(angleCube1, 1.0f, 0.0f, 0.0f);  // Base case rotate on x-axis [KHB] 

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.2f, 1.0f, 0.2f);     //[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(0.0f, 0.5f, 0.0f);//[KHB] 
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.5f, 0.0f); //[KHB] 
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(0.5f, 1.0f, 0.0f);  //[KHB] 
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);//[KHB] 
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(0.5f, 0.5f, 0.5f);//[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube

	// Render another cube
	glLoadIdentity();                  // Reset the model-view matrix
	glTranslatef(0.0f, 0.0f, -10.0f);  // center cube [KHB] 

	//Take user input axis [KHB] 
	if (cube2axis == 'x')
		glRotatef(angleCube2, 1.0f, 0.0f, 0.0f);  // Rotate about (1,0,0) x-axis [KHB]
	else if (cube2axis == 'y')
		glRotatef(angleCube2, 0.0f, 1.0f, 0.0f);  // Rotate about (0,1,0) y-axis [KHB] 
	else if (cube2axis == 'z')
		glRotatef(angleCube2, 0.0f, 0.0f, 1.0f);  // Rotate about (0,0,1) z-axis [KHB]
	else
		glRotatef(angleCube2, 0.0f, 1.0f, 0.0f);  // Base case rotate on y-axis [KHB] 

	//[KHB] 
	glBegin(GL_QUADS);           // Begin drawing the color cube with 6 quads [KHB] 
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f); //[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.8f, 1.0f); //[KHB] 
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.2f, 0.2f); //[KHB] 
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.5f); //[KHB] 
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f); //[KHB] 
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.5f, 1.0f); //[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube

	// Render a third cube
	glLoadIdentity();                  // Reset the model-view matrix
	glTranslatef(3.0f, 0.0f, -10.0f);  // right most cube [KHB] 

	//Take user input axis [KHB] 
	if (cube3axis == 'x')
		glRotatef(angleCube3, 1.0f, 0.0f, 0.0f);  // Rotate about (1,0,0) x-axis [KHB]
	else if (cube3axis == 'y')
		glRotatef(angleCube3, 0.0f, 1.0f, 0.0f);  // Rotate about (0,1,0) y-axis [KHB] 
	else if (cube3axis == 'z')
		glRotatef(angleCube3, 0.0f, 0.0f, 1.0f);  // Rotate about (0,0,1) z-axis [KHB]
	else
		glRotatef(angleCube3, 0.0f, 0.0f, 1.0f);  // Base case rotate on z-axis [KHB] 

											  //[KHB] 
	glBegin(GL_QUADS);           // Begin drawing the color cube with 6 quads [KHB] 
								 // Top face (y = 1.0f)
								 // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.2f, 0.2f, 0.4f);    //[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.3f);     //[KHB] 
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.1f, 0.7f);     //[KHB] 
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.6f);     //[KHB] 
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.3f, 0.3f, 1.0f);     //[KHB] 
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.2f, 1.0f);     //[KHB] 
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube


	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

	
						
	// Update the rotational angle after each refresh [KHB] 
	angleCube1 += 0.2f;
	angleCube2 -= 0.15f;
	angleCube3 += 0.1f; 
}

/* Called back when timer expired //[KHB]  */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei w, GLsizei h) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (h == 0) 
		h = 1;                // To prevent divide by 0

	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	// Set the viewport to cover the new window
	glViewport(0, 0, w, h);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

	//Take user input for rotational axis information on each cube [KHB] 
	cout << "Please enter x, y, or z for Cube #1: "; 
	cin >> cube1axis; 
	cout << "Please enter x, y, or z for Cube #2: "; 
	cin >> cube2axis; 
	cout << "Please enter x, y, or z for Cube #3: "; 
	cin >> cube3axis; 



	//Initialize after taking in axis choices for all three cubes 
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(1500, 700);   // Set the window's initial width & height [KHB] 
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title 




	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event




	//Start rendering 
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(0, timer, 0);     // First timer call immediately [KHB] 
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}