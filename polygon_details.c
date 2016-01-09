#include <stdio.h>
#include <GL/glut.h>



int initWindow(char *title);
void init(void);
void display(void);
void reshape(int w,int h);

int main(int argc,char **argv){
	glutInit(&argc,argv);
	initWindow("Polygon Details");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
	}
	
int initWindow(char *title){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200,1000);
	glutInitWindowPosition(0,0);
	glutCreateWindow(title);
	return 0;
	}
	
void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	}
	
void display(void){
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	*/
	for(i=0;i<5;i++){
		glBegin(GL_POLYGON);
			glVertex2f(100.0+i*210,100.0);
			glVertex2f(100.0+i*210,300.0);
			glVertex2f(300.0+i*210,100.0);
		glEnd();
	}
	/*glBegin(GL_LINE_STRIP);
	for(i=0;i<100;i++){
			glVertex2f(100.0+i*10,100.0);
			glVertex2f(105.0+i*10,110.0);
			//glVertex2f(110.0+i*10,100.0);
	}
	glEnd();
	*/
	/*glDisable(GL_CULL_FACE);*/
	glFlush();
	}
	
void reshape(int w,int h){	
	glViewport(0,0,(GLsizei) w,(GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble) w,0.0, (GLdouble) h);
	}

