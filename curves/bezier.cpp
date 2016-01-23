
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <new>
#include <math.h>
#include <GL/glut.h>
#include "bezier.h"

/* Constructor */
Bezier::Bezier(GLint num_points,Point *user_points){
	this->num_points = num_points;
	this->user_points = user_points;
	this->comp_points = NULL;
	}

/* Computes Bezier curve points */	
GLint Bezier::compute(GLint n){
	/* Memory is allocated, free it before assigning new */
	GLfloat coeff[this->num_points],t;
	GLint i = 0;
	
	/* Compute Bezier coefficients */
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
	for(i=0;i<=n-1;i++){
		t = (GLfloat) i / (n-1);
		compute_point(t,coeff,&(this->comp_points[i]));
		log_D("(" << this->comp_points[i].x <<"," << this->comp_points[i].y << ")");
		}
	return 0;
	}

/* Compute bezier coefficients */	
GLint Bezier::compute_coeff(GLfloat *coeff){
		GLint i = 0, j =0, n = this->num_points-1;
		log_D("Coefficients: ");
		std::cout << std::fixed;
		for(i=0;i<=n/2;i++){
			coeff[i]=coeff[n-i]=1;
			for(j=1;j<=i;j++){
				coeff[i] = coeff[i] * (GLfloat)(n-j+1)/j; 
				}
			coeff[n-i]=coeff[i];
			log_D(coeff[i]);
			}
	return 0;		
	}
	
/* Compute single point on bezier curve */
GLint Bezier::compute_point(GLfloat t,const GLfloat *coeff,Point *p){
	GLint i,n=this->num_points-1;
	GLfloat temp=0;
	p->x = 0;
	p->y = 0;
	for(i=0;i<=n;i++){
		temp = coeff[i] * pow(t,i) * pow(1-t,n-i);
		p->x += temp * this->user_points[i].x;
		p->y += temp * this->user_points[i].y; 
		}
	return 0;	
	}
	
/* Render bezier curve */ 	
GLint Bezier::display(){
	GLint i=0;
	glBegin(GL_LINE_STRIP);
	for(i=0;i<num_comp_points;i++){
			glVertex2f(this->comp_points[i].x,this->comp_points[i].y);
		}
	glEnd();
	return 0;
	}

