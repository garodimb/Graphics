#include <GL/glut.h>
#include <cstring>
#include <SceneNode.h>

SceneNode::SceneNode()
{
	local_trans_mat = new float[16]();
	float identity [] ={
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0
						};
	memcpy(local_trans_mat,identity,16*sizeof(float));
	parent = NULL;
	modelscale = new Vector;
	modelscale->x = modelscale->y = modelscale->z = 1;
}

SceneNode::~SceneNode()
{
	cout<<"I am in destructort"<<endl;
	delete local_trans_mat;
	parent = NULL;
	delete modelscale;
}

int SceneNode::add_child(SceneNode *node)
{
	child.push_back(node);
	node->parent = this;
	return 0;
}

int SceneNode::detach()
{
	parent->detach_child(this);
	parent = NULL;
	return 0;
}

int SceneNode::set_transf(float *mat)
{
	if(mat!=NULL){
		memcpy(local_trans_mat,mat,16*sizeof(float));
		}
	return 0;
}

int SceneNode::update_transf(float *mat)
{
	cout<<"Update Transformation matrix"<<endl;
	return 1;
}

int SceneNode::detach_child(SceneNode *node)
{
	child.erase(remove(child.begin(), child.end(), node), child.end());;
	return 0;
}

int SceneNode::display()
{
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_COLOR_MATERIAL);	// specify object color
	glColor3f(1.0, 0.1, 0.1);		// redish
	glPushMatrix();
	glMultMatrixf(local_trans_mat);
	glutWireTeapot(0.5);
	glPopMatrix();
	for( std::vector<SceneNode *>::iterator iter = child.begin(); iter != child.end(); ++iter ){
		(*iter)->display();
	}
	return 0;
}