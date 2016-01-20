
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

#define DELTA_POINT 5.0

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
int addVertex(GLint x,GLint y);
int delete_vertex(GLint x,GLint y);
int displayVertex();
int draw_axis();
int plot_curves();

/* Global Variables */
Point points[MAX_VERTICES];
GLint vCount=-1;
GLint num_points=100;
int screenWidth,screenHeight;
int IN_CURVE_MODE=FALSE;
int INS_MODE=TRUE;
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
		log_D("Left button Pressed at ("<< x <<","<< y <<")");
		
		if(!IN_CURVE_MODE && INS_MODE){
			/* In Point Mode and insert vertex mode */
			addVertex(x-TRANS_X,screenHeight-y-TRANS_Y);
			}
		else if(!IN_CURVE_MODE && !INS_MODE){
				/* In Point mode and delete vertex mode */
			delete_vertex(x-TRANS_X,screenHeight-y-TRANS_Y);
			}
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		log_D("Right button Pressed at ("<< x <<","<< y <<")");
		/* In Curve mode, Pick and translate */
		if(IN_CURVE_MODE){
			
			}
		}
	}

/* Keyboard Handler */	
void keyboardHandelr(unsigned char key,int x,int y){
	log_D("Key Pressed: " << (int)key);
	if(key==KEY_ESC){
		log_I("ESC key pressed. Exiting Program");
		exit(0);
		}
	else if(key==KEY_ENTER && vCount>=1){
		IN_CURVE_MODE=!IN_CURVE_MODE;
		if(IN_CURVE_MODE){
			log_I("Switching to CURVE mode...");
			plot_curves();
			}
		else{
			log_I("Switching to POINT mode");
			log_I("Setting INSERT mode for vertex");
			INS_MODE = TRUE;
			glMatrixMode (GL_MODELVIEW);
			glLoadIdentity();
			}
		glutPostRedisplay();
		}
	else if(key==ZOOMIN && IN_CURVE_MODE){
		log_I("Zooming IN");
		scale(ZOOMIN_FACTOR_X,ZOOMIN_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==ZOOMOUT && IN_CURVE_MODE){
		log_I("Zooming OUT");
		scale(ZOOMOUT_FACTOR_X,ZOOMOUT_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==KEY_L && IN_CURVE_MODE){
		log_I("Rotating LEFT");
		rotate(ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==KEY_R && IN_CURVE_MODE){
		log_I("Rotating RIGHT");
		rotate(-ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==KEY_H && IN_CURVE_MODE){
		log_I("Setting Translation mode to HORIZONTAL");
		TRANS_MODE = TRANS_MODE_H;
		}
	else if(key==KEY_V && IN_CURVE_MODE){
		log_I("Setting Translation mode to VERTICAL");
		TRANS_MODE = TRANS_MODE_V;
		}
	else if(key == KEY_D && IN_CURVE_MODE){
		log_I("Setting Translation mode to DIAGONAL");
		TRANS_MODE = TRANS_MODE_D;
		}
	else if(key == KEY_I && !IN_CURVE_MODE){
		log_I("Setting INSERT mode for vertex");
		INS_MODE = TRUE;
		}
	else if(key == KEY_D && !IN_CURVE_MODE){
		log_I("Setting DELETE mode for vertex");
		INS_MODE = FALSE;
		}
	}
	
/* Keyboard Special keys handler */	
void keyboardSpecHandelr(int key,int x,int y){
	if(key==GLUT_KEY_UP){
		log_I("Translating UP");
		translate(10.0);
		glutPostRedisplay();
		}
	else if(key==GLUT_KEY_DOWN){
		log_I("Translating DOWN");
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
	log_D("Handling reshape");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	log_D("Window Width: "<<w<<", Height: "<<h);
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glTranslatef(TRANS_X,TRANS_Y,0.0);
	glMatrixMode (GL_MODELVIEW);
    //glLoadIdentity();
	screenHeight=h;
	screenWidth=w;	
	}
	
/* Display Handler */
void display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT);
	draw_axis();
	glColor3f(1.0,0.0,0.0);
	if(IN_CURVE_MODE){
		log_D("Displaying Curves");
		bezier->display();
		glColor3f(0.0,1.0,0.0);
		lagrange->display();
		}
	displayVertex();
	glutSwapBuffers();
	}

/* Draw Reference axis */	
int draw_axis(){
	log_D("Drawing axis");
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
	log_D("Displaying vertex");
	int i=0;
	glBegin(GL_POINTS);
		for(i=0;i<=vCount;i++){
			glVertex2f((GLint)points[i].x,(GLint)points[i].y);
			}
	glEnd();
	return 0;	
	}

/* Add control point */	
int addVertex(GLint x,GLint y){
	log_I("Adding vertex at ("<<x<<","<<y<<")");
	vCount++;
	points[vCount].x=x;
	points[vCount].y=y;
	glutPostRedisplay();
	return 0;
	}
	
int delete_vertex(GLint x,GLint y){
	int i =0,pos = -1; 
	for(i=0;i<=vCount;i++){
		if(abs((points[i].x - x) <= DELTA_POINT) && (abs(points[i].y - y) <= DELTA_POINT)){
			log_I("Deleting vertex at ("<<(GLint)points[i].x<<","<<(GLint)points[i].y<<")");
			pos = i;
			vCount--;
			break;
			}
		}
	if(pos<0){
		return 0;
		}
	for(i = pos; i<=vCount ;i++){
		points[i]=points[i+1];
		}
	glutPostRedisplay();
	return 0;
	}
	
/* Plot curves */
int plot_curves(){
	delete bezier;
	bezier = NULL;
	bezier = new Bezier(vCount+1,points);
	if(bezier==NULL){
		log_E("Unable to allocate memory for Bezier curve");
		exit(1);
		}
	bezier->compute(num_points);
	
	delete lagrange;
	lagrange = NULL;
	lagrange = new Lagrange(vCount+1,points);
	if(lagrange==NULL){
		log_E("Unable to allocate memory for Lagrange curve");
		exit(1);
		}
	lagrange->compute(num_points);
	return 0;
	}
	
int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("Bezier and Lagrange curve",1000,500);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboardHandelr);
	glutSpecialFunc(keyboardSpecHandelr);
	glutMainLoop();
	return 0;
	}
