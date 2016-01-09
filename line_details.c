/**
 * Draw different types of lines using pattern, i.e. stipple 16 bit pattern.
 * Lower bit of pattern used first
 * Stipple resets after each new line and continuous for segments(GL_LINE_STRIP)
 * We can sepcifiy repeating pattern of stipple, where every bit is repeated that many time. Each bit corresponds to one pixel
 * Specify different width of line
 * Antializing is disabled by default so pixel is of square
 */

#include <stdio.h>
#include <GL/glut.h>

/* Macro to draw one Line */
#define drawOneLine(x1,y1,x2,y2) glBegin(GL_LINES); glVertex2f((x1),(y1)); glVertex2f((x2),(y2)); glEnd();

void init(void);
void display(void);
void reshape(int w,int h);
int initWindow(char *title);

int main(int argc,char **argv){
	/* Initialiaze GLUT */
	glutInit(&argc,argv);
	
	/* Initialize Window */
	initWindow("Line Details");
	
	/* Initialize */
	init();
	
	/* Register display function */
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	/* Main Loop */
	glutMainLoop();
	return 0;
	}


int initWindow(char *title){
	
	printf("In InitWindos\n");
	/* Set Display Mode */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	
	/* Window Parameters */
	glutInitWindowSize(1500,1500);
	glutInitWindowPosition(200,200);
	
	/* Draw Window */
	glutCreateWindow(title);
	return 0;
	}

void init(void){
	
	/* Set Clear Color */
	printf("In Init\n");
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	
	}
	
void reshape(int w,int h){
	printf("In Reshape\n");
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble) w,0.0, (GLdouble) h);
	}

void display(void){
	int i;
	printf("In Display\n");
	/* Clear Screen */
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Set drawing color RGB */
	glColor3f(1.0,0.0,0.0);
	
	/* Draw Lines */
	glEnable(GL_LINE_STIPPLE);
	
	glLineStipple(1,0x0101);
	drawOneLine(50.0,400.0,250.0,400.0);
	glLineStipple(1,0x00FF);
	drawOneLine(250.0,400.0,450.0,400.0);
	glLineStipple(1,0x1C47);
	drawOneLine(450.0,400.0,650.0,400.0);
	
	glLineWidth(5.0);
	glLineStipple(1,0x0101);
	drawOneLine(50.0,300.0,250.0,300.0);
	glLineStipple(1,0x00FF);
	drawOneLine(250.0,300.0,450.0,300.0);
	glLineStipple(1,0x1C47);
	drawOneLine(450.0,300.0,650.0,300.0);
	
	glLineStipple(4,0x1C47);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<4;i++){
		glVertex2f(50.0 + ((GLfloat) i * 200.0),200.0);
		}
	glEnd();
	/* Flush, does not wait to complete drawing on screen, glFinish() waits to complete drawing on screen */
	
	glDisable(GL_LINE_STIPPLE);
	glFlush();
	}
