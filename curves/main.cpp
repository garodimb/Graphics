
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "bezier.h"

#define MAX_VERTICES 100
#define FALSE 0
#define TRUE 1

using namespace std;

int initWindow(char *title,int w,int h);
void init(void);
void reshape(int w,int h);
void display(void);
void mouseHandler(int button,int state,int x,int y);
void keyboardHandelr(unsigned char key,int x,int y);
int addVertex(int x,int y);
int displayVertex();

Point points[MAX_VERTICES];
GLint vCount=-1;
int screenWidth,screenHeight;
int IN_CURVE_MODE=FALSE;

Bezier *bezier;

int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(w,h);
	screenWidth=w;
	screenHeight=h;
	glutInitWindowPosition(20,20);
	glutCreateWindow(title);
	return 0;
	}
	
void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(5.0);
	glShadeModel(GL_FLAT);	
	}

void mouseHandler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		cout<<"Left button Pressed at ("<<x<<","<<y<<")"<<endl;
		if(!IN_CURVE_MODE)
			addVertex(x,screenHeight-y);
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

void keyboardHandelr(unsigned char key,int x,int y){
	cout<<"Key Pressed: "<<(int)key<<endl;
	if(key==KEY_ESC){
		cout<<"ESC key pressed. Exiting Program"<<endl;
		exit(0);
		}
	else if(key==KEY_ENTER){
		IN_CURVE_MODE=!IN_CURVE_MODE;
		if(IN_CURVE_MODE){
			/* Delete if any preallocated Memory */
			delete bezier;
			bezier = NULL;
			bezier = new Bezier(vCount+1,points);
			if(bezier==NULL){
				cout<<"Unable to allocate memory for curve"<<endl;
				exit(1);
				}
			bezier->compute(100);
			}
		glutPostRedisplay();
		}
	}

void reshape(int w,int h){
	cout<<"In Reshape"<<endl;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
	screenHeight=h;
	screenWidth=w;	
	}
	
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	cout<<"In Display"<<endl;
	glColor3f(1.0,0.0,0.0);
	if(IN_CURVE_MODE){
		cout<<"Displaying Curve"<<endl;
		bezier->display();
		}
	displayVertex();
	glutSwapBuffers();
	}
	
int displayVertex(){
	int i=0;
	glBegin(GL_POINTS);
		for(i=0;i<=vCount;i++){
			glVertex2f((GLint)points[i].x,(GLint)points[i].y);
			}
	glEnd();
	return 0;	
	}
	
int addVertex(int x,int y){
	vCount++;
	points[vCount].x=x;
	points[vCount].y=y;
	cout<<"Adding vertx("<<x<<","<<y<<")"<<endl;
	glutPostRedisplay();
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
	glutMainLoop();
	return 0;
	}
