#include "model.h"
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

long Model::nvertices 	= 0;
long Model::ntriangles	= 0;
long Model::count_v 	= 0;
long Model::count_t		= 0;

vertex *Model::vertices = NULL;
triangle *Model::triangles = NULL;
normal *Model::nml = NULL;

int Model::vertex_cb(p_ply_argument argument) {
	static int pos = 0;
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    switch (pos) {
		case 0:
				vertices[count_v].x = ply_get_argument_value(argument);
				break;
		case 1:
				vertices[count_v].y = ply_get_argument_value(argument);
				break;
		case 2:
				vertices[count_v].z = ply_get_argument_value(argument);
				count_v++;
				break;
		default:
				break;
			
		}
	pos = (pos+1)%3;
    return 1;
	}

int Model::face_cb(p_ply_argument argument) {
    long length, value_index;
    int x,y,z;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    switch (value_index) {
        case 0:
				triangles[count_t].x = (int)ply_get_argument_value(argument);
				break;
        case 1:
				triangles[count_t].y = (int)ply_get_argument_value(argument);
				break;
        case 2:
				triangles[count_t].z = (int)ply_get_argument_value(argument);
				compute_normal(count_t); // Compute normal after all vertices of traingle are known
				count_t++;
				break;
        default:
				break;
		}
		return 1;
	}

int Model::init(char filename[]){
    p_ply ply = ply_open(filename, NULL, 0, NULL);
    if (!ply) return 1;
    if (!ply_read_header(ply)) return 1;
    nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
    ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
    count_t = 0;
    count_v = 0;
    if(vertices != NULL){
		delete [] vertices;
		}
    if(triangles != NULL){
		delete [] triangles;
		}
	if(nml != NULL){
		delete [] nml;
		}
	vertices = new vertex[nvertices];
	triangles = new triangle[ntriangles];
	nml = new normal[ntriangles];
    if (!ply_read(ply)) return 1;
    ply_close(ply);
    return 0;
	}

int Model::compute_normal(int pos){
	vec3 v1,v2;
	// v1 = p2 - p1; v2 = p3 -p2
	v1.x = vertices[triangles[pos].y].x - vertices[triangles[pos].x].x; // a1
	v1.y = vertices[triangles[pos].y].y - vertices[triangles[pos].x].y; // a2
	v1.z = vertices[triangles[pos].y].z - vertices[triangles[pos].x].z; // a3
	
	v2.x = vertices[triangles[pos].z].x - vertices[triangles[pos].y].x; //b1
	v2.y = vertices[triangles[pos].z].y - vertices[triangles[pos].y].y; //b2
	v2.z = vertices[triangles[pos].z].z - vertices[triangles[pos].y].z; //b3

	//Matrix Multiplication
	
	//Cross Product: (a2b3-a3b2)i - (a1b3 - a3b1)j + (a1b2 - a2b1)k
	//Here changing to original size giving normals inward
	nml[pos].x = -(v1.y*v2.z - v1.z*v2.y);
	nml[pos].y = (v1.x*v2.z - v1.z*v2.x);
	nml[pos].z = -(v1.x*v2.y - v1.y*v2.x);
	return 0;
	}
	
int Model::display(){
	long i = 0;
	glColor3f(1.0,1.0,1.0);
	for(i=0;i<ntriangles;i++){
		glBegin(GL_TRIANGLES);
			glNormal3f(nml[i].x,nml[i].y,nml[i].z);
			glVertex3f(vertices[triangles[i].x].x,vertices[triangles[i].x].y,vertices[triangles[i].x].z);
			glVertex3f(vertices[triangles[i].y].x,vertices[triangles[i].y].y,vertices[triangles[i].y].z);
			glVertex3f(vertices[triangles[i].z].x,vertices[triangles[i].z].y,vertices[triangles[i].z].z);
		glEnd();
		}
	return 0;
	}
