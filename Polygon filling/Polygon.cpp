//Polygon.cpp


//#include "Polygon.h"

//#ifndef _WINDEF_
//
//#include "stdafx.h"
//#endif

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "Polygon.h"

void Polygon::initPolygon( int maxVertices)
{

	this->maxVertices = maxVertices;

	this->vertices = new Point[maxVertices];

	this->numVertices = 0;


}


void Polygon::addVertex( float x, float y)
{

	if (numVertices <= maxVertices) 
	{
		vertices[numVertices].x = x;
		vertices[numVertices].y = y;
		numVertices++;
	}
	else 
		printf("\n Cant add mode vertices...");

}


void Polygon::draw(const bool FILL_FLAG)
{


	if (!FILL_FLAG)
		glBegin(GL_LINE_LOOP);

	else
		glBegin(GL_POLYGON);
	
	//printf("\nDrawing points: ");

	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++ )		
		glVertex2f(  vertices[vertexIndex].x, vertices[vertexIndex].y );
	
		//printf(" (%d, %d) ", vertices[vertexIndex].x, vertices[vertexIndex].y );

	glEnd();
}


void Polygon::drawPoints() 
{
	//printf("\nIn DrawPoints\n");
	glBegin(GL_POINTS);
	for(int vertexIndex =0; vertexIndex < numVertices; vertexIndex++ ) 
		glVertex2f(vertices[vertexIndex].x, vertices[vertexIndex].y );
	glEnd();
	glFlush();
}



void Polygon::drawLabels() {


	for(int vertexIndex =0; vertexIndex < numVertices; vertexIndex++ ) {
	
	//	glPushMatrix();
	//	glTranslatef(vertices[vertexIndex].x, vertices[vertexIndex].y,0.0 );

		glRasterPos2f(vertices[vertexIndex].x, vertices[vertexIndex].y);
		//glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_10, vertexIndex + 'A');
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, vertexIndex + 'A');

	//	glPopMatrix();
	}

}

