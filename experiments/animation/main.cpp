
#include <iostream>
#include <GL/glut.h>
#include <SceneNode.h>

SceneNode * scene;
using namespace std;
/* Initialize windowing system */
int initWindow(const char *title,int w,int h)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(100,100);
	glutCreateWindow(title);
	return 0;
}

/* Init canvas */
void init(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(3.0);
	glEnable(GL_NORMALIZE); //Dont care about performance now
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

/* Respahe window */
void reshape(int w,int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, (float)w/h, 0.1f,15.0f);
}

int init_model()
{
	float trans_mat [] ={
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.8, 0.0, 0.0, 1.0
						};
	scene = new SceneNode();
	scene->set_transf(trans_mat);
	trans_mat[12] = -0.8;
	SceneNode *teapot = new SceneNode();
	teapot->set_transf(trans_mat);
	scene->add_child(teapot);
	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0,-5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	scene->display();
	glutSwapBuffers();
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	initWindow("Animation Demo",1000,500);
	init();
	init_model();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}