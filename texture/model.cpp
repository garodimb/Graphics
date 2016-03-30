#include <model.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <string.h>
#include <GL/glut.h>

/**************************************************************************
 * Temporary section
 **************************************************************************/
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

/**************************************************************************
 * Model Section
 **************************************************************************/
PlyProperty vert_props[] = { /* list of property information for a vertex */
  {(char *)"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
  {(char *)"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
  {(char *)"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0},
};

typedef int Vertex_Indices[4];

PlyProperty face_props[] = { /* list of property information for a vertex */
  {(char *)"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
   1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)},
};

Model::Model()
{
	x_min = y_min = z_min = INT_MAX;
	x_max = y_max = z_max = INT_MIN;
	nvertices = ntriangles = 0;
	vlist = NULL;
	flist = NULL;
	normal = NULL;
}

Model::~Model()
{

}

int Model::read_ply(char *fn)
{
  int i,j,k;
  PlyFile *ply;
  int nelems;
  char **elist;
  int file_type;
  float version;
  int nprops;
  int num_elems;
  PlyProperty **plist;
  char *elem_name;
  int num_obj_info;
  char **obj_info;

  /* open a PLY file for reading */
  ply = ply_open_for_reading(fn, &nelems, &elist, &file_type, &version);
  for (i = 0; i < nelems; i++) {

    /* get the description of the first element */
    elem_name = elist[i];
    plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

    /* if we're on vertex elements, read them in */
    if (equal_strings ((char *)"vertex", elem_name)) {

      /* create a vertex list to hold all the vertices */
      vlist = new Vertex*[num_elems];
      nvertices = num_elems;
      /* set up for getting vertex elements */

      ply_get_property (ply, elem_name, &vert_props[0]);
      ply_get_property (ply, elem_name, &vert_props[1]);
      ply_get_property (ply, elem_name, &vert_props[2]);

      /* grab all the vertex elements */
      for (j = 0; j < num_elems; j++) {

        /* grab and element from the file */
        vlist[j] = new Vertex;
        ply_get_element (ply, (void *) vlist[j]);

		/* Find Max value */
        if(vlist[j]->x > x_max){
			x_max = vlist[j]->x;
			}
		if(vlist[j]->y > y_max){
			y_max = vlist[j]->y;
			}
		if(vlist[j]->z > z_max){
			z_max = vlist[j]->z;
			}

		/* Find Min value */
		if(vlist[j]->x < x_min){
			x_min = vlist[j]->x;
			}
		if(vlist[j]->y < y_min){
			y_min = vlist[j]->y;
			}
		if(vlist[j]->z < z_min){
			z_min = vlist[j]->z;
			}
      }
    }

    /* if we're on face elements, read them in */
    if (equal_strings ((char *)"face", elem_name)) {

      /* create a list to hold all the face elements */
      flist = new Face*[num_elems];
      ntriangles = num_elems;

      /* set up for getting face elements */

	  ply_get_property (ply, elem_name, &face_props[0]);
      /* grab all the face elements */
      for (j = 0; j < num_elems; j++) {

        /* grab and element from the file */
        flist[j] = new Face;
        ply_get_element (ply, (void *) flist[j]);
      }
    }
  }
  ply_close (ply);
  compute_normal();
  init_tex();
  compute_tex_cord();
  return 0;
}

int Model::compute_normal(){
	Vector v1, v2;
	int i;
	float x,y,z;
	// v1 = p2 - p1; v2 = p3 -p2
	normal = new Vector*[nvertices];
	for(i=0;i<nvertices;i++){
		normal[i] = new Vector;
		normal[i]->x = 0;
		normal[i]->y = 0;
		normal[i]->z = 0;
		}

	for(i=0;i<ntriangles;i++){
		v1.x = vlist[flist[i]->verts[1]]->x - vlist[flist[i]->verts[0]]->x; // a1
		v1.y = vlist[flist[i]->verts[1]]->y - vlist[flist[i]->verts[0]]->y; // a2
		v1.z = vlist[flist[i]->verts[1]]->z - vlist[flist[i]->verts[0]]->z; // a3

		v2.x = vlist[flist[i]->verts[2]]->x - vlist[flist[i]->verts[1]]->x; // a1
		v2.y = vlist[flist[i]->verts[2]]->y - vlist[flist[i]->verts[1]]->y; // a2
		v2.z = vlist[flist[i]->verts[2]]->z - vlist[flist[i]->verts[1]]->z; // a3

		//Matrix Multiplication

		//Cross Product: (a2b3-a3b2)i - (a1b3 - a3b1)j + (a1b2 - a2b1)k
		//Here changing to original size giving normals inward
		x =  (v1.y*v2.z - v1.z*v2.y);
		y = -(v1.x*v2.z - v1.z*v2.x);
		z =  (v1.x*v2.y - v1.y*v2.x);
		normal[flist[i]->verts[0]]->x += x;
		normal[flist[i]->verts[0]]->y += y;
		normal[flist[i]->verts[0]]->z += z;

		normal[flist[i]->verts[1]]->x += x;
		normal[flist[i]->verts[1]]->y += y;
		normal[flist[i]->verts[1]]->z += z;

		normal[flist[i]->verts[2]]->x += x;
		normal[flist[i]->verts[2]]->y += y;
		normal[flist[i]->verts[2]]->z += z;
		}
	return 0;
	}

int Model::init_tex()
{
	glGenTextures(1, &tex_name);
	glBindTexture(GL_TEXTURE_2D, tex_name);
	makeCheckImage();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                checkImage);
	return 0;
}
int Model::compute_tex_cord()
{
	float mag;
	tex_cord = new TexCord*[nvertices];
	for(int i=0;i<nvertices;i++){
		mag = sqrt(normal[i]->x*normal[i]->x+normal[i]->y*normal[i]->y+normal[i]->z*normal[i]->z);
		normal[i]->x = (float)normal[i]->x/mag;
		normal[i]->y = (float)normal[i]->y/mag;
		normal[i]->z = (float)normal[i]->z/mag;
		tex_cord[i] = new TexCord;
		tex_cord[i]->u = normal[i]->x / 2 + 0.5;
		tex_cord[i]->v = normal[i]->y / 2 + 0.5;
		//cout<<i<<"  "<<tex_cord[i]->u<<"  "<<tex_cord[i]->v<<endl;
		}
	return 0;
}
int Model::display(){
	int i;
	float scale_factor = 0.0f;

	/* Dynamic Scaling Factor */
	if((float )fabs(x_max - x_min)/2.0f > scale_factor){
		scale_factor = (float )fabs(x_max - x_min)/2.0f;
		}
	if((float )fabs(y_max - y_min)/2.0f > scale_factor){
		scale_factor = (float )fabs(y_max - y_min)/2.0f;
		}
	if((float )fabs(z_max - z_min)/2.0f > scale_factor){
		scale_factor = (float )fabs(z_max - z_min)/2.0f;
		}
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	if(scale_factor==0)
		scale_factor = 1.0f;
	glScalef(1.0f/scale_factor,1.0f/scale_factor,1.0f/scale_factor);

	glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, tex_name);
/***********************************************************************
 * Automatic Texture generation
 **********************************************************************/
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
/***********************************************************************
 *
 **********************************************************************/
	glTranslatef((float)-(x_min + x_max)/2.0f,(float)-(y_min + y_max)/2.0f,(float)-(z_min + z_max)/2.0f);
	for(i=0;i<ntriangles;i++){
		glBegin(GL_POLYGON);
				glNormal3f(normal[flist[i]->verts[0]]->x,normal[flist[i]->verts[0]]->y,normal[flist[i]->verts[0]]->z);
				//glTexCoord2f(tex_cord[flist[i]->verts[0]]->u,tex_cord[flist[i]->verts[0]]->v);
				glVertex3f(vlist[flist[i]->verts[0]]->x,vlist[flist[i]->verts[0]]->y,vlist[flist[i]->verts[0]]->z);

				glNormal3f(normal[flist[i]->verts[1]]->x,normal[flist[i]->verts[1]]->y,normal[flist[i]->verts[1]]->z);
				//glTexCoord2f(tex_cord[flist[i]->verts[1]]->u,tex_cord[flist[i]->verts[1]]->v);
				glVertex3f(vlist[flist[i]->verts[1]]->x,vlist[flist[i]->verts[1]]->y,vlist[flist[i]->verts[1]]->z);

				glNormal3f(normal[flist[i]->verts[2]]->x,normal[flist[i]->verts[2]]->y,normal[flist[i]->verts[2]]->z);
				//glTexCoord2f(tex_cord[flist[i]->verts[2]]->u,tex_cord[flist[i]->verts[2]]->v);
				glVertex3f(vlist[flist[i]->verts[2]]->x,vlist[flist[i]->verts[2]]->y,vlist[flist[i]->verts[2]]->z);
		glEnd();
		}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	return 0;
	}
