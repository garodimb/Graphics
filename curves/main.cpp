
#include <stdio.h>
#include <iostream>
#include <math.h>
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
void mouse_handler(int button,int state,int x,int y);
void keyboard_handler(unsigned char key,int x,int y);
void keyboard_spec_handler(int key,int x,int y);

/* Transformations */
int trans_by_user(GLint x,GLint y);
int scale(GLfloat x_factor,GLfloat y_factor);
int rotate(GLfloat angle);
int translate(GLfloat dist);

/* UI Related */
int add_vertex(GLint x,GLint y);
int delete_vertex(GLint x,GLint y);
int display_vertex();
int draw_axis();
int plot_curves();

/* Global Variables */
Point points[MAX_VERTICES];
Point trans_point;

int v_count=-1; // Number of vertices added by user
int t_point_count=-1; //Track of first point in user defined translation
int num_points=100; //Number of points to be calculated on curve
int screenWidth,screenHeight;
int IN_CURVE_MODE=FALSE; //Curve mode or vertex mode
int TRANS_MODE=TRANS_MODE_D;

/* Curves */
Bezier *bezier;
Lagrange *lagrange;

/* Initialize windowing system */
int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(w,h);
	screenWidth=w;
	screenHeight=h;
	glutInitWindowPosition(100,100);
	glutCreateWindow(title);
	return 0;
	}
	
/* Init canvas */	
void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(3.0);
	glShadeModel(GL_FLAT);	
	}

/* Mouse Handler */
void mouse_handler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		log_D("Left button Pressed at ("<< x <<","<< y <<")");
		
		if(!IN_CURVE_MODE){
			/* In Vertex mode */
			add_vertex(x-TRANS_X,screenHeight-y-TRANS_Y);
			}
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		log_D("Right button Pressed at ("<< x <<","<< y <<")");
		if(IN_CURVE_MODE){
			/* In Curve mode, Pick and translate */
			trans_by_user(x-TRANS_X,screenHeight-y-TRANS_Y);
			}
		else{
			/* In vertex mode, delete points */
			delete_vertex(x-TRANS_X,screenHeight-y-TRANS_Y);
			}
		}
	}

/* Keyboard Handler */	
void keyboard_handler(unsigned char key,int x,int y){
	log_D("Key Pressed: " << (int)key);
	if(key==KEY_ESC){
		log_I("ESC key pressed. Exiting Program");
		exit(0);
		}
	else if(key==KEY_ENTER && v_count>=1){
		IN_CURVE_MODE=!IN_CURVE_MODE;
		if(IN_CURVE_MODE){
			log_I("Switching to CURVE mode...");
			plot_curves();
			}
		else{
			log_I("Switching to VERTEX mode");
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
	}
	
/* Keyboard Special keys handler */	
void keyboard_spec_handler(int key,int x,int y){
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

/* Do Translation by user defined distance */
int trans_by_user(GLint x,GLint y){
	GLfloat x_dist,y_dist,dist;
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
	screenHeight=h;
	screenWidth=w;	
	}
	
/* Display Handler */
void display(){
	log_D("Displaying content");
	glClear(GL_COLOR_BUFFER_BIT);
	draw_axis();
	if(IN_CURVE_MODE){
		log_D("Displaying Curves");
		glColor3f(0.0,1.0,1.0);
		lagrange->display();
		glColor3f(1.0,0.0,1.0);
		bezier->display();
		}
	glColor3f(0.0,1.0,1.0);
	display_vertex();
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
		glVertex2f(500.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,500.0);		
	glEnd();
	glPopMatrix();
	return 0;
	}
	
/* Display control points */	
int display_vertex(){
	log_D("Displaying vertex");
	int i=0;
	glBegin(GL_POINTS);
		for(i=0;i<=v_count;i++){
			glVertex2f((GLint)points[i].x,(GLint)points[i].y);
			}
	glEnd();
	return 0;	
	}

/* Add control point */	
int add_vertex(GLint x,GLint y){
	log_I("Adding vertex at ("<<x<<","<<y<<")");
	v_count++;
	points[v_count].x=x;
	points[v_count].y=y;
	glutPostRedisplay();
	return 0;
	}

/* Delete vertex */
int delete_vertex(GLint x,GLint y){
	int i =0,pos = -1; 
	for(i=0;i<=v_count;i++){
		if(abs((points[i].x - x) <= DELTA_POINT) && (abs(points[i].y - y) <= DELTA_POINT)){
			log_I("Deleting vertex at ("<<(GLint)points[i].x<<","<<(GLint)points[i].y<<")");
			pos = i;
			v_count--;
			break;
			}
		}
	if(pos<0){
		return 0;
		}
	for(i = pos; i<=v_count ;i++){
		points[i]=points[i+1];
		}
	glutPostRedisplay();
	return 0;
	}
	
/* Plot curves */
int plot_curves(){
	delete bezier;
	bezier = NULL;
	bezier = new Bezier(v_count+1,points);
	if(bezier==NULL){
		log_E("Unable to allocate memory for Bezier curve");
		exit(1);
		}
	bezier->compute(num_points);
	
	delete lagrange;
	lagrange = NULL;
	lagrange = new Lagrange(v_count+1,points);
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
	glutMouseFunc(mouse_handler);
	glutKeyboardFunc(keyboard_handler);
	glutSpecialFunc(keyboard_spec_handler);
	glutMainLoop();
	return 0;
	}
