#include "controller.h"
#include "model.h"
#include "view.h"

int main(int argc,char **argv){
	if(argc==2){
		Model::init(argv[1]);
		}
	else{
		Model::init((char *)"input.ply");
		}
	View *view = new View(argc,argv);
	Controller *controller = new Controller(view);
	glutMainLoop();
	return 0;
	}
