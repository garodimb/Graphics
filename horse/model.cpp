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
	vertices = new vertex[nvertices];
	triangles = new triangle[ntriangles];
    if (!ply_read(ply)) return 1;
    ply_close(ply);
    return 0;
	}
	
int Model::display(){
	long i = 0;
	for(i=0;i<ntriangles;i++){
		glBegin(GL_TRIANGLES);
			glVertex3f(vertices[triangles[i].x].x,vertices[triangles[i].x].y,vertices[triangles[i].x].z);
			glVertex3f(vertices[triangles[i].y].x,vertices[triangles[i].y].y,vertices[triangles[i].y].z);
			glVertex3f(vertices[triangles[i].z].x,vertices[triangles[i].z].y,vertices[triangles[i].z].z);
		glEnd();
		}
	return 0;
	}
