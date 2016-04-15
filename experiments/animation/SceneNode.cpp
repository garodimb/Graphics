#include <GL/glut.h>
#include <cstring>
#include <SceneNode.h>
#include <log.h>

/*
 * Constructor
 */
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
	this->parent = NULL;
	this->model = NULL;
	this->modelscale = new Vector;
	if(!(this->modelscale)){
		log_E("Unable to allocate memory to modelscale");
		}
	modelscale->x = modelscale->y = modelscale->z = 1;
}
/*
 * Destructor
 */
SceneNode::~SceneNode()
{
	log_D("Freeing memory");
	delete local_trans_mat;
	parent = NULL;
	delete modelscale;
	for( std::vector<SceneNode *>::iterator iter = child.begin(); iter != child.end(); ++iter ){
		delete (*iter);
	}
}
/*
 * Set model
 */
int SceneNode::set_model(Model *model)
{
	this->model = model;
	return 0;
}
/*
 * Get pointer to model
 */
Model * SceneNode::get_model()
{
	return model;
}

/*
 * Set scale factor for current model only and not
 * for its childern
 */
int SceneNode::scale(const Vector& modelscale)
{
	this->modelscale->x = modelscale.x;
	this->modelscale->y = modelscale.y;
	this->modelscale->z = modelscale.z;
	return 0;
}

/*
 * Add child to current node
 */
int SceneNode::add_child(SceneNode *node)
{
	child.push_back(node);
	node->parent = this;
	return 0;
}
/*
 * Set new transformation matrix by copying values
 */
int SceneNode::set_transf(float *mat)
{
	if(mat!=NULL){
		memcpy(local_trans_mat,mat,16*sizeof(float));
		}
	return 0;
}

/*
 * Update Transformation Matrix by multiplying with
 * current matrix
 */
int SceneNode::update_transf(float *mat)
{
	log_D("Update Transformation matrix");
	return 1;
}

/*
 * Detach current node from its parent
 * and call detach_child of parent
 */
int SceneNode::detach()
{
	parent->detach_child(this);
	parent = NULL;
	return 0;
}

/*
 * Detach node child from parent and delete its entry from
 * child vector. This function should be get called from
 * child's detach function
 */
int SceneNode::detach_child(SceneNode *node)
{
	child.erase(remove(child.begin(), child.end(), node), child.end());;
	return 0;
}

/*
 * Display scene in perorder manner
 * pushMatrix();
 * MulMatrix();
 * current model
 * child display();
 * popMatrix();
 */

int SceneNode::display()
{
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_COLOR_MATERIAL);	// specify object color
		glColor3f(1.0, 0.1, 0.1);		// redish
		glPushMatrix();
		glMultMatrixf(local_trans_mat);
	//This can be special node without model
	if(model!=NULL){
		//Scaling only current model and not child
		glPushMatrix();
		glScalef(modelscale->x,modelscale->y,modelscale->z);
		glPopMatrix();
		glutWireTeapot(0.5);
		}
	// Display all child
	for( std::vector<SceneNode *>::iterator iter = child.begin(); iter != child.end(); ++iter ){
		(*iter)->display();
	}
	glPopMatrix();
	return 0;
}