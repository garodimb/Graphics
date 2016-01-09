#include <stdio.h>
#include <GL/glut.h>

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
	}
	
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
	glLineWidth(15.0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0.1,0.6);
		glVertex2f(0.2,0.4);
		glVertex2f(0.4,0.4);
		glVertex2f(0.3,0.6);
	glEnd();
	glFlush();
	}

int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(250,250);
	glutInitWindowPosition(500,500);
	glutCreateWindow("Ploygon Demo");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
	}
