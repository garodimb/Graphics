//testPoly.cpp

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
//#include <iostream.h>

#include <GL/glut.h>
#include <math.h>


#include "Polygon.h"
//#include "Polygon.cpp"

const int screenWidth=640;// width of screen window in pixels
const int screenHeight=480;// height of scrren window in pixels
GLdouble A,B,C,D;// values used for scaling and shifting


const int MAX_NUM_VERTICES = 100;

Polygon p(MAX_NUM_VERTICES);

bool SHOW_POLYGON = false;
bool FILL_POLYGON = false;

enum Menu {POINTS, LINES, FILL, LABEL };

Menu currChoice;


//****************** myInit ******************/

void myInit(void)
{

	glClearColor(0, 0, 0,0.0);// set whitebackground color
	glColor3f(1.0f,0.0f,0.0f);// set the drawing color(black)
	glPointSize(2.0);// a 'dot' is 4 by 4 pixels
	glMatrixMode(GL_PROJECTION);// set camera shape
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)screenWidth,0.0,(GLdouble)screenHeight);
	A = screenWidth/4;// set values used for scaling and shifting
	B = 0;
	C = D = screenHeight/2;
}


//****************** myDisplay ******************/

void myDisplay(void) 
{


//	gluOrtho2D(0,screenWidth, 0, screenHeight);

	glClear(GL_COLOR_BUFFER_BIT);// clear the screen

	
	
	/*glBegin(GL_POINTS);

	for(GLdouble x=0;x<4;x+=0.005) {
		GLdouble func=exp(-x) * cos(2*3.14159265*x);
		glVertex2d(A*x+B,C*func+D);
	}

	glEnd();*/

	if (currChoice == POINTS) p.drawPoints();
	if (currChoice == LINES) p.draw(0);
	if (currChoice == FILL) 
	{
		glColor3f(0.0f,0.0f,1.0f);
		p.draw(1);
	}
	//if (currChoice == LABEL) p.drawLabels();

	//p.drawPoints();

	//if ( SHOW_POLYGON ) {
	//	p.draw(FILL_POLYGON);
	//}

	glFlush();// send all output to display
}


void drawText()
{
	glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10, 'V');
	glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10, 'I');
	glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10, 'N');
}

//***************** myKeyHandler **********************

void myKeyHandler( unsigned char key, int x, int y) 
{


	if (key == 27)
	{
		// Hitting ESC quits
		exit(1);
	}

	printf("\n key pressed is %c at (%d, %d)", key , x, y);

}

// ***************************************************

void myMouseHandler( int button, int state, int x, int y ) 
{


	if (state == GLUT_DOWN) 
	{
		printf("\nAdding vertex (%d, %d) to polygon", x, screenHeight- y);
		p.addVertex(  x, screenHeight- y);
		p.drawPoints();
	}

	//p.drawPoints();
}
// ****************************************************

void myMenu( int value ) 
{

	printf("\nSelected menu value = %d", value);
	

	/*if (value ==1)
	{ // 
	//	SHOW_POLYGON = true;
	//	FILL_POLYGON = false;
		currChoice = POINTS;
		myDisplay();
	}*/

	if (value ==2 ) 
	{
	//	SHOW_POLYGON = false;
	//	FILL_POLYGON = false;
		currChoice = LINES;
		myDisplay();
	}
	

	if (value==3) 
	{
	//	SHOW_POLYGON = true;
	//	FILL_POLYGON = true;
		currChoice = FILL;
		myDisplay();
	}

	/*if (value == 4) 
	{
		currChoice = LABEL;
		myDisplay();
	}*/


	if (value == 0) 
	{ // exit
		exit(1);
	}

}
//*********************** main *******************
int main(int argc, char* argv[])
{

	glutInit(&argc,argv);// Initialize  the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);// set Display mode
	glutInitWindowSize(640,480);// set window size
	glutInitWindowPosition(100,150);// set window position on screen
	glutCreateWindow("Polygon filling");// open the screen window
	
	
	glutDisplayFunc(myDisplay);// register redrtaw function

	glutKeyboardFunc(myKeyHandler);  // register key handler

	glutMouseFunc(myMouseHandler);


	// set up menu
	const int MenuID = glutCreateMenu (myMenu); 
	//glutAddMenuEntry("Dots", 1);
	glutAddMenuEntry("Lines", 2);
	glutAddMenuEntry("Fill",3);
	//glutAddMenuEntry("Vertex labels",4);
	glutAddMenuEntry( "Exit", 0);



	glutAttachMenu(GLUT_RIGHT_BUTTON);





	myInit();	
	//printf("Hello World!\n");
	glutMainLoop();

	return 0;
}

