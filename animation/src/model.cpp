#include <model.h>
#include <stdio.h>
#include <cmath>
#include <limits.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <GL/glut.h>

static float	plane_xy[3] = {1, 0, 0};
static float	plane_yz[3] = {0, 0, 1};

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

Model::Model(string &fn,Map map,string &tex_path)
{
	this->fn = fn;
	this->map = map;
	this->tex_path = tex_path;
	x_min = y_min = z_min = INT_MAX;
	x_max = y_max = z_max = INT_MIN;
	trans_x = trans_z = 0;
	tex_mode = 1;
	nvertices = ntriangles = 0;
	vlist = NULL;
	flist = NULL;
	normal = NULL;
	scale_factor = 0.0f;
	centroid.x = centroid.y = centroid.z = 0.0f;
	read_ply();
	compute_scale_factor();
	compute_normal();
	init_tex();
	if(map == Sphere){
		compute_sphere_cord();
	}
	else{
		compute_cyl_cord();
	}
}

Model::~Model()
{
	for(int i=0;i<nvertices;i++){
		delete vlist[i];
		delete normal[i];
		delete tex_cord[i];
		}
	for(int i=0;i<ntriangles;i++){
		delete flist[i];
		}

	delete [] vlist;
	delete [] flist;
	delete [] normal;
	delete [] tex_cord;
}

int Model::read_ply()
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
  ply = ply_open_for_reading(const_cast<char*>(fn.c_str()), &nelems, &elist, &file_type, &version);
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
  return 0;
}

int Model::compute_scale_factor()
{
	/* Dynamic Scaling Factor */
	/*scale_factor = 0.0f;
	if((float )fabs(x_max - x_min)/1.0f > scale_factor){
		scale_factor = (float )fabs(x_max - x_min)/1.0f;
		}
	if((float )fabs(y_max - y_min)/1.0f > scale_factor){
		scale_factor = (float )fabs(y_max - y_min)/1.0f;
		}
	if((float )fabs(z_max - z_min)/1.0f > scale_factor){
		scale_factor = (float )fabs(z_max - z_min)/1.0f;
		}*/
	scale_factor = 1.0;
	centroid.x = (float)(x_max + x_min)/2.0f;
	centroid.y = (float)(y_max + y_min)/2.0f;
	centroid.z = (float)(z_max + z_min)/2.0f;
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
	texture = new Texture(tex_path);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)texture->get_width(),
                (GLsizei)texture->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                (GLubyte *)texture->get_data());

	return 0;
}


int Model::compute_sphere_cord()
{
	float mag;
	tex_cord = new TexCord*[nvertices];
	for(int i=0;i<nvertices;i++){
		Vector v;
		v.x = (vlist[i]->x - centroid.x);
		v.y = (vlist[i]->y - centroid.y);
		v.z = (vlist[i]->z - centroid.z);
		mag = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
		tex_cord[i] = new TexCord;
		/* This is not efficient mapping of texture*/
		tex_cord[i]->u = 0.5 + (float)atan2f(v.z/mag,v.x/mag) / 2 * M_PI;
		tex_cord[i]->v = 0.5 - (float)asinf(v.y/mag) / M_PI;
		}
	return 0;
}

int Model::compute_cyl_cord()
{
	tex_cord = new TexCord*[nvertices];
	float mag;
	for(int i=0;i<nvertices;i++){
		tex_cord[i] = new TexCord;
		/* This is not efficient mapping of texture */
		mag = sqrtf(vlist[i]->x * vlist[i]->x+ vlist[i]->y * vlist[i]->y + vlist[i]->z * vlist[i]->z);
		tex_cord[i]->u = (float)atan2f(vlist[i]->x/mag,vlist[i]->z/mag)/ 2 * M_PI + 0.5;
		tex_cord[i]->v = (float)vlist[i]->y/y_max;
		}
	return 0;
}

int Model::display(){
	int i;

	enable_tex();
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	if(scale_factor==0)
		scale_factor = 1.0f;
	glScalef(1.0f/scale_factor,1.0f/scale_factor,1.0f/scale_factor);
	glTranslatef(-centroid.x,-centroid.y,-centroid.z);
	float new_trans =(centroid.y - y_min)/scale_factor - 2.0;
	glTranslatef(trans_x*scale_factor,new_trans*scale_factor,trans_z*scale_factor);
	for(i=0;i<ntriangles;i++){
		glBegin(GL_POLYGON);
				glNormal3f(normal[flist[i]->verts[0]]->x,normal[flist[i]->verts[0]]->y,normal[flist[i]->verts[0]]->z);
				glTexCoord2f(tex_cord[flist[i]->verts[0]]->u,tex_cord[flist[i]->verts[0]]->v);
				glVertex3f(vlist[flist[i]->verts[0]]->x,vlist[flist[i]->verts[0]]->y,vlist[flist[i]->verts[0]]->z);

				glNormal3f(normal[flist[i]->verts[1]]->x,normal[flist[i]->verts[1]]->y,normal[flist[i]->verts[1]]->z);
				glTexCoord2f(tex_cord[flist[i]->verts[1]]->u,tex_cord[flist[i]->verts[1]]->v);
				glVertex3f(vlist[flist[i]->verts[1]]->x,vlist[flist[i]->verts[1]]->y,vlist[flist[i]->verts[1]]->z);

				glNormal3f(normal[flist[i]->verts[2]]->x,normal[flist[i]->verts[2]]->y,normal[flist[i]->verts[2]]->z);
				glTexCoord2f(tex_cord[flist[i]->verts[2]]->u,tex_cord[flist[i]->verts[2]]->v);
				glVertex3f(vlist[flist[i]->verts[2]]->x,vlist[flist[i]->verts[2]]->y,vlist[flist[i]->verts[2]]->z);
		glEnd();
		}
	glPopMatrix();
	disable_tex();
	return 0;
	}

int Model::update_tex(string& tex_path)
{
	delete texture;
	this->tex_path = tex_path;
	log_D("Changing texture to: "<<tex_path);
	init_tex();
	if(map == Sphere){
		compute_sphere_cord();
	}
	else{
		compute_cyl_cord();
	}
	return 0;
}

int Model::enable_tex()
{
	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glScalef (1, -1, 1);
	glBindTexture(GL_TEXTURE_2D, tex_name);
	//Automatic Mode
	if(tex_mode==0){
		if(map = Sphere){
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
			}
		glTexGenfv (GL_S, GL_EYE_PLANE, plane_xy);
		glTexGenfv (GL_T, GL_EYE_PLANE, plane_yz);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	glEnable(GL_TEXTURE_2D);
	return 0;
}

int Model::disable_tex()
{
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_2D);
	return 0;
}

int Model::update_tex_mode(int mode)
{
	tex_mode = mode;
	return 0;
}

int Model::trans_obj_by(float trans_x,float trans_z)
{
	this->trans_x += trans_x;
	this->trans_z += trans_z;
	return 0;
}

int Model::trans_obj_to(float trans_x,float trans_z)
{
	this->trans_x = trans_x;
	this->trans_z = trans_z;
	return 0;
}