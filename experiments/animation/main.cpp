
#include <iostream>
#include <GL/glut.h>
#include <SceneNode.h>
#include <log.h>
#include <math.h>
#include <unistd.h>
SceneNode * scene;
using namespace std;
/* Initialize windowing system */
SceneNode *teapot1,*teapot2,*teapot3;
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
	teapot1 = new SceneNode();
	char *model = new char();
	*model = 'Y';
	/*
	 * Create some model here and organize in well
	 * tree base format to understand scene graph
	 */
	teapot1->set_model(model);
	teapot2 = new SceneNode();
	teapot2->set_model(model);
	teapot3 = new SceneNode();
	teapot3->set_model(model);
	trans_mat[12] = -1.6;
	scene->add_child(teapot1);
	teapot2->set_transf(trans_mat);
	teapot1->add_child(teapot2);
	trans_mat[13] = 0.8;
	teapot3->set_transf(trans_mat);
	teapot2->add_child(teapot3);
	teapot1->add_child(teapot3);
	return 0;
}
/*
 * IdleFunc for creating animation
 * Movement of models
 */
void idleFunc()
{
	static float count=0.0f;
	static int flag = 0;
	float trans_mat [] ={
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0
						};
	trans_mat[12]+=count;
	double x;
	trans_mat[12] = modf(trans_mat[12],&x)*4;
	if(trans_mat[12]>=3.9){
		flag=1;
		}
	if(trans_mat[12]<=0){
		flag=0;
		}
	if(!flag)
		count += 0.01;
	else
		count -= 0.01;
	teapot3->set_transf(trans_mat);
	usleep(50000);
	glutPostRedisplay();
}

/*
 * Display entrie scene
 */
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
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}