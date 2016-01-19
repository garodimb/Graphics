
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "bezier.h"
#include "lagrange.h"


#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

/* Scaling Factors */
#define ZOOMIN_FACTOR_X 1.2
#define ZOOMIN_FACTOR_Y 1.2
#define ZOOMOUT_FACTOR_X 1/1.2
#define ZOOMOUT_FACTOR_Y 1/1.2

/* Rotation Angle */
#define ROTATE_ANGLE 5.0

/* Initial Translation for visibility of Ref axes */
#define TRANS_X 20.0
#define TRANS_Y 20.0

#define TRANS_MODE_H 1
#define TRANS_MODE_V 2
#define TRANS_MODE_D 3

using namespace std;

/* Window init */
int initWindow(char *title,int w,int h);
void init(void);
void reshape(int w,int h);
void display(void);

/* Callbacks */
void mouseHandler(int button,int state,int x,int y);
void keyboardHandelr(unsigned char key,int x,int y);
void keyboardSpecHandelr(int key,int x,int y);

/* Transformations */
int scale(GLfloat x_factor,GLfloat y_factor);
int rotate(GLfloat angle);
int translate(GLfloat dist);

/* UI Related */
int addVertex(int x,int y);
int displayVertex();
int draw_axis();
int plot_curves();

/* Global Variables */
Point points[MAX_VERTICES];
GLint vCount=-1;

int screenWidth,screenHeight;
int IN_CURVE_MODE=FALSE;
int TRANS_MODE=TRANS_MODE_D;
GLfloat TRANS_DIST=10.0;

/* Curves */
Bezier *bezier;
Lagrange *lagrange;

/* Initialize windowing system */
int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(w,h);
	screenWidth=w;
	screenHeight=h;
	glutInitWindowPosition(20,20);
	glutCreateWindow(title);
	return 0;
	}
	
/* Init canvas */	
void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(5.0);
	glShadeModel(GL_FLAT);	
	}

/* Mouse Handler */
void mouseHandler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		cout<<"Left button Pressed at ("<<x<<","<<y<<")"<<endl;
		if(!IN_CURVE_MODE)
			addVertex(x-TRANS_X,screenHeight-y-TRANS_Y);	
		}
	else if(button==GLUT_LEFT_BUTTON && state == GLUT_UP){
		cout<<"Left button Released at ("<<x<<","<<y<<")"<<endl;
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		cout<<"Right button Pressed at ("<<x<<","<<y<<")"<<endl;
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_UP){
		cout<<"Right button Released at ("<<x<<","<<y<<")"<<endl;
		}
	}

/* Keyboard Handler */	
void keyboardHandelr(unsigned char key,int x,int y){
	cout<<"Key Pressed: "<<(int)key<<endl;
	if(key==KEY_ESC){
		cout<<"ESC key pressed. Exiting Program"<<endl;
		exit(0);
		}
	else if(key==KEY_ENTER && vCount>=1){
		IN_CURVE_MODE=!IN_CURVE_MODE;
		if(IN_CURVE_MODE){
			/* Delete if any preallocated Memory */
			plot_curves();
			}
		else{
			glMatrixMode (GL_MODELVIEW);
			glLoadIdentity();
			}
		glutPostRedisplay();
		}
	else if(key==ZOOMIN && IN_CURVE_MODE){
		cout<<"Zooming in"<<endl;
		scale(ZOOMIN_FACTOR_X,ZOOMIN_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==ZOOMOUT && IN_CURVE_MODE){
		cout<<"Zooming out"<<endl;
		scale(ZOOMOUT_FACTOR_X,ZOOMOUT_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==KEY_L && IN_CURVE_MODE){
		cout<<"Rotating left"<<endl;
		rotate(ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==KEY_R && IN_CURVE_MODE){
		cout<<"Rotating right"<<endl;
		rotate(-ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==KEY_H && IN_CURVE_MODE){
		TRANS_MODE = TRANS_MODE_H;
		}
	else if(key==KEY_V && IN_CURVE_MODE){
		TRANS_MODE = TRANS_MODE_V;
		}
	else if(key == KEY_D && IN_CURVE_MODE){
		TRANS_MODE = TRANS_MODE_D;
		}
	}
	
/* Keyboard Special keys handler */	
void keyboardSpecHandelr(int key,int x,int y){
	if(key==GLUT_KEY_UP){
		translate(10.0);
		glutPostRedisplay();
		}
	else if(key==GLUT_KEY_DOWN){
		translate(-10.0);
		glutPostRedisplay();
		}
	}

/* Scaling */
int scale(GLfloat x_factor,GLfloat y_factor){
	glTranslatef(points[0].x,points[0].y,0.0);
	glScalef(x_factor,y_factor,0.0);
	glTranslatef(-points[0].x,-points[0].y,0.0);
	return 0;
	}
	
/* Rotation */
int rotate(GLfloat angle){
	glTranslatef(points[0].x,points[0].y,0.0);
	glRotatef(angle,0.0,0.0,1.0);
	glTranslatef(-points[0].x,-points[0].y,0.0);
	return 0;
	}
	
/* Translation */
int translate(GLfloat dist){
	if(TRANS_MODE==TRANS_MODE_H)
		glTranslatef(dist,0.0,0.0);
	else if(TRANS_MODE==TRANS_MODE_V)
		glTranslatef(0.0,dist,0.0);
	else
		glTranslatef(dist,dist,0.0);
	return 0;
	}	
	
/* Reshape Handler */
void reshape(int w,int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glTranslatef(TRANS_X,TRANS_Y,0.0);
	glMatrixMode (GL_MODELVIEW);
    //glLoadIdentity();
	screenHeight=h;
	screenWidth=w;	
	}
/* Display Handler */
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	cout<<"In Display"<<endl;
	draw_axis();
	glColor3f(1.0,0.0,0.0);
	if(IN_CURVE_MODE){
		cout<<"Displaying Curve"<<endl;
		bezier->display();
		glColor3f(0.0,1.0,0.0);
		lagrange->display();
		}
	displayVertex();
	glutSwapBuffers();
	}

/* Draw Reference axis */	
int draw_axis(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,500.0);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(500.0,0.0);
	glEnd();
	glPopMatrix();
	return 0;
	}
	
/* Display control points */	
int displayVertex(){
	int i=0;
	glBegin(GL_POINTS);
		for(i=0;i<=vCount;i++){
			glVertex2f((GLint)points[i].x,(GLint)points[i].y);
			}
	glEnd();
	return 0;	
	}

/* Add control point */	
int addVertex(int x,int y){
	vCount++;
	points[vCount].x=x;
	points[vCount].y=y;
	cout<<"Adding vertx("<<x<<","<<y<<")"<<endl;
	glutPostRedisplay();
	return 0;
	}
/* Plot curves */
int plot_curves(){
	delete bezier;
	bezier = NULL;
	bezier = new Bezier(vCount+1,points);
	if(bezier==NULL){
		cout<<"Unable to allocate memory for curve"<<endl;
		exit(1);
		}
	bezier->compute(100);
	
	delete lagrange;
	lagrange = NULL;
	lagrange = new Lagrange(vCount+1,points);
	if(lagrange==NULL){
		cout<<"Unable to allocate memory for curve"<<endl;
		exit(1);
		}
	lagrange->compute(100);
	return 0;
	}
	
int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("Mouse and Keyboard Handler",1000,500);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboardHandelr);
	glutSpecialFunc(keyboardSpecHandelr);
	glutMainLoop();
	return 0;
	}
