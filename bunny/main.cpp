#include "controller.h"
#include "model.h"
#include "view.h"

int main(int argc,char **argv){
	Model::init((char *)"bunny.ply");
	View *view = new View(argc,argv);
	Controller *controller = new Controller(view);
	glutMainLoop();
	return 0;
	}
