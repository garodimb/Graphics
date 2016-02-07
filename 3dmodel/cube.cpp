#include <stdio.h>
#include <iostream>
#include <iomanip>   
#include <new>
#include <math.h>
#include <GL/glut.h>
#include <string.h>
#include "cube.h"

/* Constructor */
Cube::Cube(){
	GLint def_faces[6][4] = { { 0 ,1, 2, 3 }, { 4 ,5, 6, 7 }, { 3 ,2, 6, 7 }, { 0 ,1, 5, 4 }, { 1 ,2, 6, 5 }, { 0 ,3, 7, 4 } };
	GLfloat color[8][3]   = { {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0,1.0,1.0}, {1.0,0.0,0.0}, {1.0,0.0,1.0},{1.0,1.0,0.0}, {1.0,1.0,1.0}};
	GLfloat normal[6][3]  = { { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0 }};
	v[0][0] = v[1][0] = v[4][0] = v[5][0] =   0.5;
	v[2][0] = v[3][0] = v[6][0] = v[7][0] =  -0.5;
	v[1][1] = v[2][1] = v[5][1] = v[6][1] =   0.5;
	v[0][1] = v[3][1] = v[4][1] = v[7][1] =  -0.5;
	v[4][2] = v[5][2] = v[6][2] = v[7][2] =   0.5;
	v[0][2] = v[1][2] = v[2][2] = v[3][2] =  -0.5;
	
	
	memcpy(this->faces,def_faces,sizeof(def_faces));
	memcpy(this->c,color,sizeof(color));
	memcpy(this->n,normal,sizeof(normal));
	}

GLint Cube::display(){
	GLint i = 0,j=0;
	for(i=0;i<NUM_FACES;i++){
		glBegin(GL_POLYGON);
		glNormal3fv(this->n[i]);
		for(j=0;j<4;j++){
			glColor3fv(this->c[this->faces[i][j]]);
			glVertex3fv(this->v[this->faces[i][j]]);
			}
		glEnd();
		}
	return 0;
	}
