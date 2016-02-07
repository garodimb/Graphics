/* Map window coordinates to object coordinates
 * Application: Picking points on the fly */

void project(int x_cursor, int y_cursor){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX,winY;
	GLdouble x,y,z;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	// obtain the Z position (not world coordinates but in range 0 - 1)
	GLfloat z_cursor;
	winX = (float)x_cursor;
	winY = (float)viewport[3]-(float)y_cursor;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);

	// obtain the world coordinates
	gluUnProject(winX, winY, z_cursor, modelview, projection, viewport, &x, &y, &z);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glColor3f(1.0,1.0,1.0);
		glVertex3d(x,y,z);
	glEnd();
}

/* ArcBall Program for rotation */
/* https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball */
