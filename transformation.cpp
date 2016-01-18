/* Transformations
 * 1. Rotation
 * 2. Scaling
 */

#include <stdio.h>
#include <GL/glut.h>

#include <iostream>

#define KEY_ENTER 13 //Switch Mode Key
#define KEY_ESC 27 //Exit Application Key
#define ZOOMIN 43
#define ZOOMOUT 45
#define KEY_L 108 // Left Rotation
#define KEY_R 114 // Right Rotation
#define ROTATE_ANGLE 5.0

/* Zoom Factors */
#define ZOOMIN_FACTOR_X 1.2
#define ZOOMIN_FACTOR_Y 1.2
#define ZOOMOUT_FACTOR_X 1/1.2
#define ZOOMOUT_FACTOR_Y 1/1.2

/* Traslation factor */
#define TRANS_X 10.0
#define TRANS_Y 10.0

using namespace std;

int initWindow(char *title,int w,int h);
void init(void);
void reshape(int w,int h);
void display(void);
void mouseHandler(int button,int state,int x,int y);
void keyboardHandelr(unsigned char key,int x,int y);
void keyboardSpecHandelr(int key,int x,int y);

GLint scale(GLfloat x_factor,GLfloat y_factor);
GLint rotate(GLfloat angle);
GLint translate(GLfloat x,GLfloat y);

GLint screenWidth,screenHeight;
void draw_axis();

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
		/* Zoom IN */
		scale(ZOOMIN_FACTOR_X,ZOOMIN_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(button==GLUT_LEFT_BUTTON && state == GLUT_UP){
		cout<<"Left button Released at ("<<x<<","<<y<<")"<<endl;
		}
	else if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		cout<<"Right button Pressed at ("<<x<<","<<y<<")"<<endl;
		/* Zoom Out */
		scale(ZOOMOUT_FACTOR_X,ZOOMOUT_FACTOR_Y);
		glutPostRedisplay();
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
	else if(key==KEY_L){
		/* Left Rotation */
		rotate(ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==KEY_R){
		/* Right Rotation */
		rotate(-ROTATE_ANGLE);
		glutPostRedisplay();
		}
	else if(key==ZOOMIN){
		/* Zoom in */
		scale(ZOOMIN_FACTOR_X,ZOOMIN_FACTOR_Y);
		glutPostRedisplay();
		}
	else if(key==ZOOMOUT){
		/* Zoom out */
		scale(ZOOMOUT_FACTOR_X,ZOOMOUT_FACTOR_Y);
		glutPostRedisplay();
		}
	}
	
void keyboardSpecHandelr(int key,int x,int y){
	if(key==GLUT_KEY_LEFT){
		translate(-TRANS_X,0.0);
		glutPostRedisplay();		
		}
	else if(key==GLUT_KEY_RIGHT){
		translate(TRANS_X,0.0);
		glutPostRedisplay();
		}
	else if(key==GLUT_KEY_UP){
		translate(0.0,TRANS_Y);
		glutPostRedisplay();
		}
	else if(key==GLUT_KEY_DOWN){
		translate(0.0,-TRANS_Y);
		glutPostRedisplay();
		}
	}

void reshape(int w,int h){
	cout<<"In Reshape"<<endl;
	/* Set ViewPort */
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	
	/* Orthographic projection */
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	
	/* Ref. axes visibility translation */
	glTranslatef(20.0,20.0,0.0);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	screenHeight=h;
	screenWidth=w;	
	}
	
GLint scale(GLfloat x_factor,GLfloat y_factor){
	/* Move object to origin, scale and change to original origin */
	glTranslatef(100.0,100.0,0.0);
	glScalef(x_factor,y_factor,0.0);
	glTranslatef(-100.0,-100.0,0.0);
	return 0;
	}
	
GLint rotate(GLfloat angle){
	/* Move object to origin, rotate and change to original origin */
	glTranslatef(100.0,100.0,0.0);
	glRotatef(angle,0.0,0.0,1.0);
	glTranslatef(-100.0,-100.0,0.0);
	return 0;
	}

GLint translate(GLfloat x,GLfloat y){
	glTranslatef(x,y,0.0);
	return 0;
	}
	
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	cout<<"In Display"<<endl;
	glColor3f(1.0,0.0,0.0);
	/* Print Filled Triangle */	
	glBegin(GL_TRIANGLES);
		glVertex2f(100.0,100.0);
		glVertex2f(100.0,250.0);
		glVertex2f(250.0,100.0);
	glEnd();
	draw_axis();
	glutSwapBuffers();
	}

void draw_axis(){
	/* Load ModelView Matrix if not loaded, push current matrix 
	 * Translation of Ref axis is taken care by Ref axis visibility translation
	 */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,500.0);
		glVertex2f(0.0,0.0);
		glVertex2f(500.0,0.0);
	glEnd();
	glPopMatrix();
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
