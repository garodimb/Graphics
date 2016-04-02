#include <iostream>
#include <iomanip>
#include <new>
#include <cmath>
#include <GL/glut.h>
#include <cstring>
#include <cube.h>


static float	plane_xy[3] = {1, 0, 0};
static float	plane_yz[3] = {0, 0, 1};

/* Constructor */
Cube::Cube(float dim){
	GLint def_faces[6][4] = { { 0 ,1, 2, 3 }, { 4 ,5, 6, 7 }, { 3 ,2, 6, 7 }, { 0 ,1, 5, 4 }, { 1 ,2, 6, 5 }, { 0 ,3, 7, 4 } };
	GLfloat color[8][3]   = { {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0,1.0,1.0}, {1.0,0.0,0.0}, {1.0,0.0,1.0},{1.0,1.0,0.0}, {1.0,1.0,1.0}};
	GLfloat normal[6][3]  = { { 0.0, 0.0, -1.0 }, { 0.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0 }};
	v[0][0] = v[1][0] = v[4][0] = v[5][0] =   (float) dim/2.0;
	v[2][0] = v[3][0] = v[6][0] = v[7][0] =  (float) -dim/2.0;
	v[1][1] = v[2][1] = v[5][1] = v[6][1] =   (float) dim/2.0;
	v[0][1] = v[3][1] = v[4][1] = v[7][1] =  (float) -dim/2.0;
	v[4][2] = v[5][2] = v[6][2] = v[7][2] =   (float) dim/2.0;
	v[0][2] = v[1][2] = v[2][2] = v[3][2] =  (float) -dim/2.0;
	init_tex();
	memcpy(this->faces,def_faces,sizeof(def_faces));
	memcpy(this->c,color,sizeof(color));
	memcpy(this->n,normal,sizeof(normal));
	}

Cube::Cube(){
	Cube(1.0f);
	}

Cube::~Cube()
{
	delete [] walls;
	delete [] floor;
	delete [] ceiling;
}

int Cube::init_tex()
{
	glGenTextures(1, &tex_name[0]);
	glBindTexture(GL_TEXTURE_2D, tex_name[0]);
	string fn = "texfiles/plank01.bmp";
	walls = new Texture(fn);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)walls->get_width(),
                (GLsizei)walls->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                (GLubyte *)walls->get_data());
	return 0;
}

GLint Cube::display(){
	GLint i = 0,j=0;
	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glScalef (1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glBindTexture(GL_TEXTURE_2D, tex_name[0]);
	glTexGenfv (GL_S, GL_EYE_PLANE, plane_xy);
	glTexGenfv (GL_T, GL_EYE_PLANE, plane_yz);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	i = BOTTOM;
//	for(i=0;i<NUM_FACES;i++){
		glBegin(GL_POLYGON);
		glNormal3fv(this->n[i]);
		for(j=0;j<4;j++){
			//glColor3fv(this->c[this->faces[i][j]]);
			glVertex3fv(this->v[this->faces[i][j]]);
			}
		glEnd();
		//}
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_2D);
	return 0;
	}
