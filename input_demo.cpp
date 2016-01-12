#include <stdio.h>
#include <GL/glut.h>

#include <iostream>

#define KEY_ESC 27
#define KEY_ENTER 13

#define MAX_VERTICES 100
using namespace std;

int initWindow(char *title,int w,int h);
void init(void);
void reshape(int w,int h);
void display(void);
void mouseHandler(int button,int state,int x,int y);
void keyboardHandelr(unsigned char key,int x,int y);
int addVertex(int x,int y);
int drawPolygon();

struct Point{
	int x, y;	
	};


Point points[MAX_VERTICES];
int vCount=-1;
int screenWidth,screenHeight;

int initWindow(const char *title,int w,int h){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w,h);
	screenWidth=w;
	screenHeight=h;
	glutInitWindowPosition(20,20);
	glutCreateWindow(title);
	return 0;
	}
	
void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(2.0);
	glShadeModel(GL_FLAT);	
	}

void mouseHandler(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		cout<<"Left button Pressed at ("<<x<<","<<y<<")"<<endl;
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
	if(key==27){
		cout<<"ESC key pressed. Exiting Program"<<endl;
		exit(0);
		}
	else if(key==KEY_ENTER){
		drawPolygon();
		}
	}	


void reshape(int w,int h){
	cout<<"In Reshape"<<endl;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	gluOrtho2D(0.0,(GLdouble) w,0.0, (GLdouble) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	screenWidth=w;
	screenHeight=h;	
	}
	
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	cout<<"In Display"<<endl;
	glColor3f(1.0,1.0,1.0);
	drawPolygon();
	addVertex(100,100);
	glFlush();
	}
	
int addVertex(int x,int y){
	vCount++;
	points[vCount].x=x;
	points[vCount].y=y;
	glBegin(GL_POINTS);
		glVertex2d(screenWidth-x,screenHeight-y);
	glEnd();
	cout<<"Adding vertx("<<x<<","<<y<<")"<<endl;
	glFlush();
	return 0;
	}


int drawPolygon(){
	return 0;
	}
int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("Input Demo",1000,500);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboardHandelr);
	glutMainLoop();
	return 0;
	}

