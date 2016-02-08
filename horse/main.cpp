
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "quaternion.h"
#include "model.h"
#include "cube.h"

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
void mouse_handler(int button,int state,int x,int y);
void keyboard_handler(unsigned char key,int x,int y);
void keyboard_spec_handler(int key,int x,int y);

/* Transformations */
int trans_by_user(GLint x,GLint y);
int scale(GLfloat x_factor,GLfloat y_factor);
int translate(GLfloat dist);
int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);

/* UI Related */
int draw_axis();

/* Global Variables */
Point trans_point;

int screenWidth,screenHeight;
int TRANS_MODE=TRANS_MODE_D;


/*	3D Modelling */
Cube cube;
GLfloat rotate_x=0.0;
GLfloat rotate_y=0.0;
GLfloat z_distance = 3.0f;

/* Initialize windowing system */
int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	screenWidth=w;
	screenHeight=h;
	glutInitWindowPosition(100,100);
	glutCreateWindow(title);
	return 0;
	}
	
/* Init canvas */	
void init(void){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat mat_shininess[] = { 100.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 0.0 };
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel (GL_SMOOTH);
	
	/* Disable LIGHTING to view only colored cube with different colors*/
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	}

/* Mouse Handler */
void mouse_handler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		log_D("Left button Pressed at ("<< x <<","<< y <<")");
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		log_D("Right button Pressed at ("<< x <<","<< y <<")");
			/* Pick and translate */
			trans_by_user(x-TRANS_X,screenHeight-y-TRANS_Y);
		}
	}

/* Keyboard Handler */	
void keyboard_handler(unsigned char key,int x,int y){
	log_D("Key Pressed: " << (int)key);
	if(key==KEY_ESC){
		log_I("ESC key pressed. Exiting Program");
		exit(0);
		}
	else if(key==KEY_ENTER){
		log_I("Resetting MODELVIEW");
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();
		glutPostRedisplay();
		}
	else if(key==ZOOMIN){
		log_I("Zooming IN");
		z_distance-=0.1;
		//scale(ZOOMIN_FACTOR_X,ZOOMIN_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==ZOOMOUT){
		log_I("Zooming OUT");
		//scale(ZOOMOUT_FACTOR_X,ZOOMOUT_FACTOR_Y);
		z_distance+=0.1;
		glutPostRedisplay();
		}
	else if(key==KEY_H){
		log_I("Setting Translation mode to HORIZONTAL");
		TRANS_MODE = TRANS_MODE_H;
		}
	else if(key==KEY_V){
		log_I("Setting Translation mode to VERTICAL");
		TRANS_MODE = TRANS_MODE_V;
		}
	else if(key == KEY_D){
		log_I("Setting Translation mode to DIAGONAL");
		TRANS_MODE = TRANS_MODE_D;
		}
	}
	
/* Keyboard Special keys handler */	
void keyboard_spec_handler(int key,int x,int y){
	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT){
		//glRotatef( ROTATE_ANGLE, 0.0, 1.0, 0.0 );
		rotate_y += 5;
		}
	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT){
		//glRotatef( -ROTATE_ANGLE, 0.0, 1.0, 0.0 );
		rotate_y -= 5;
	}
	else if (key == GLUT_KEY_UP){
		//glRotatef( ROTATE_ANGLE, 1.0, 0.0, 0.0 );
		rotate_x += 5;
	}
	else if (key == GLUT_KEY_DOWN){
		//glRotatef( -ROTATE_ANGLE, 1.0, 0.0, 0.0 );
		rotate_x -= 5;
	}
	//  Request display update
	glutPostRedisplay();
	}

/* Do Translation by user defined distance */
int trans_by_user(GLint x,GLint y){
	GLfloat x_dist,y_dist,dist;
	static GLint t_point_count = -1;
	if(t_point_count==-1){
		/* Add first point for finding translation distance */
		trans_point.x = x;
		trans_point.y = y;
		t_point_count = 0;
		}
	else if(t_point_count==0){
		/* Calculate distance between two points */
		dist = sqrt(pow(trans_point.x - x, 2)+ pow(trans_point.y - y, 2));
		t_point_count = -1;
		
		/* Set X & Y direction */
		x_dist = ( x >= trans_point.x)? dist : -dist;
		y_dist = ( y >= trans_point.y)? dist : -dist;
		
		log_I("Translating");
		log_D("x_dist: "<<x_dist<<", y_dist: "<<y_dist);
		glTranslatef(x_dist,y_dist,0.0);
		glutPostRedisplay();
		}
	return 0;
	}
	

/* Scaling */
int scale(GLfloat x_factor,GLfloat y_factor){
	//glTranslatef(points[0].x,points[0].y,0.0);
	glScalef(x_factor,y_factor,0.0);
	//glTranslatef(-points[0].x,-points[0].y,0.0);
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

/* Rotation using Quaternion*/
int rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
	GLfloat pmatrix[16];
	Quaternion quaternion;
	quaternion.CreateFromAxisAngle(x,y,z,angle);
	quaternion.CreateMatrix(pmatrix);
	glMultMatrixf(pmatrix);
	return 0;
	}
/* Reshape Handler */
void reshape(int w,int h){
	log_D("Handling reshape");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, 1.0, 0.1f,15.0f); 
	log_D("Window Width: "<<w<<", Height: "<<h);
	screenHeight=h;
	screenWidth=w;	
	}
	
/* Display Handler */
void display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Move camera using z so to get fill of zooming */
	/* Eye, Center and Up vector */	
	gluLookAt(0.0, 0.0,z_distance, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
	rotate(1.0,0.0,0.0,rotate_x);
	rotate(0.0,1.0,0.0,rotate_y);
	//glScalef(10.0,10.0,10.0);
	cube.display();
	glScalef(5.0,5.0,5.0);
	glTranslatef(0.0,-0.1,0.0);
	Model::display();
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
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,1.0,0.0);		
	glEnd();
	glPopMatrix();
	return 0;
	}
	
int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("3D Modelling",1000,500);
	init();
	Model::init("bunny.ply");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse_handler);
	glutKeyboardFunc(keyboard_handler);
	glutSpecialFunc(keyboard_spec_handler);
	glutMainLoop();
	return 0;
	}
