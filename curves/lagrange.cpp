
#include <stdio.h>
#include <iostream>
#include <iomanip>   
#include <new>
#include <math.h>
#include <GL/glut.h>
#include "lagrange.h"

/* Constructor */
Lagrange::Lagrange(GLint num_points,Point *user_points){
	this->num_points = num_points;
	this->user_points = user_points;
	this->comp_points = NULL;
	}

/* Computes Lagrange curve points */
GLint Lagrange::compute(GLint n){
	GLint i=0;
	GLfloat coeff[this->num_points],x,delta;
	delta = (GLfloat)(this->user_points[this->num_points-1].x - this->user_points[0].x)/ (GLfloat) (n-1); 
	
	/* Compute Lagrange coefficients */
	this->compute_coeff(coeff);
	
	/* Clear any previous curve computation */
	if(this->comp_points){
		delete [] this->comp_points;
		}
	
	/* Allocate memory for computation */
	this->comp_points = new (std::nothrow) Point[n];
	if(this->comp_points==NULL){
		log_E("Unable to allocate memory for computation");
		return 1;
		}
	this->num_comp_points = n;
	x = this->user_points[0].x;
	log_D("Computing lagrange points");
	for(i=0;i<n;i++){
		compute_point(x,coeff,&(this->comp_points[i]));
		log_D("(" << this->comp_points[i].x <<"," << this->comp_points[i].y << ")");
		x+= delta;
		}
	return 0;
	}
	
/* Compute lagrange coefficients */	
GLint Lagrange::compute_coeff(GLfloat *coeff){
	GLint i=0,j=0,n=this->num_points;
	GLfloat denm = 1.0;
	log_D("Computing lagrange coefficients");
	for(i=0;i<n;i++){
			denm=1;
			for(j=0;j<n;j++){
				if(i==j)
					continue;
				denm *= (this->user_points[i].x - this->user_points[j].x);
				}
			coeff[i] = this->user_points[i].y / denm;
			log_D(coeff[i]);	
		}
	return 0;
	}

/* Compute single point on lagrange curve */	
GLint Lagrange::compute_point(GLfloat x,const GLfloat *coeff,Point *p){
	GLint i =0, j= 0, n = this->num_points;
	GLfloat num = 1.0;
	p->x = x;
	p->y = 0.0;
	
	for(i=0;i<n;i++){
		num = 1.0;
		for(j=0;j<n;j++){
			if(i==j)
				continue;
			num *= (x - this->user_points[j].x);
			}
		p->y+= num * coeff[i]; 
		}
	return 0;
	}
	
/* Render curve on screen */	
GLint Lagrange::display(){
	GLint i=0;
	glBegin(GL_LINE_STRIP);
	for(i=0;i<num_comp_points;i++){
			glVertex2f(this->comp_points[i].x,this->comp_points[i].y);
		}
	glEnd();
	return 0;
	}
