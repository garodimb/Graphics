
#include <stdio.h>
#include <iostream>
#include <iomanip>      // std::setprecision
#include <new>
#include <math.h>
#include <GL/glut.h>
#include "bezier.h"

Bezier::Bezier(GLint num_points,Point *user_points){
	this->num_points = num_points;
	this->user_points = user_points;
	this->comp_points = NULL;
	}
	
GLint Bezier::compute(GLint n){
	/* Memory is allocated, free it before assigning new */
	GLfloat coeff[this->num_points],t;
	GLint i = 0;
	this->compute_coeff(coeff);
	if(this->comp_points){
		delete [] this->comp_points;
		}
	this->comp_points = new (std::nothrow) Point[n];
	if(this->comp_points==NULL){
		std::cout<<"Unable to allocate memory for computation"<<std::endl;
		return 1;
		}
	this->num_comp_points = n;
	for(i=0;i<=n-1;i++){
		t = (GLfloat) i / (n-1);
		compute_point(t,coeff,&(this->comp_points[i]));
		std::cout <<"(" << this->comp_points[i].x <<"," << this->comp_points[i].y << ")" <<std::endl;
		}
	return 0;
	}

GLint Bezier::compute_coeff(GLfloat *coeff){
		GLint i = 0, j =0, n = this->num_points-1;
		std::cout<<"Coefficients: ";
		std::cout << std::fixed;
		for(i=0;i<=n/2;i++){
			coeff[i]=coeff[n-i]=1;
			for(j=1;j<=i;j++){
				coeff[i] = coeff[i] * (GLfloat)(n-j+1)/j; 
				}
			coeff[n-i]=coeff[i];
			std::cout << std::setprecision(5) << coeff[i]<<"  ";
			}
		std::cout<<std::endl;		
	}
	
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
	
GLint Bezier::display(){
	GLint i=0;
	glBegin(GL_LINE_STRIP);
	for(i=0;i<num_comp_points;i++){
			glVertex2f(this->comp_points[i].x,this->comp_points[i].y);
		}
	glEnd();
	return 0;
	}

