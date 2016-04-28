#include <GL/glut.h>
#include <cstring>
#include <scenenode.h>

/*
 * Constructor
 */

int SceneNode::obj_count = 0;

SceneNode::SceneNode()
{
	obj_count++;
	scene_id = obj_count+1;
	world_trans_mat = new float[16]();
	local_trans_mat = new float[16]();
	float identity [] ={
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0
						};
	memcpy(local_trans_mat,identity,16*sizeof(float));
	memcpy(world_trans_mat,identity,16*sizeof(float));
	this->parent = NULL;
	this->model = NULL;
}
/*
 * Destructor
 */
SceneNode::~SceneNode()
{
	log_D("Freeing memory");
	delete local_trans_mat;
	delete world_trans_mat;
	parent = NULL;
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
int SceneNode::set_world_transf(float *mat)
{
	if(mat!=NULL){
		memcpy(world_trans_mat,mat,16*sizeof(float));
		}
	return 0;
}

/*
 * Update Transformation Matrix by multiplying with
 * current matrix
 */
int SceneNode::update_world_transf(float *mat)
{
	Matrix mat_obj;
	mat_obj.mul_mat(world_trans_mat,mat,world_trans_mat);
	return 0;
}

 /*
 * Return value for transformation matrix
 */

int SceneNode::get_world_transf(float *mat)
{
   memcpy(mat,world_trans_mat,16*sizeof(world_trans_mat[0]));
   return 0;
}

/* Get global world transform,pass mat as Identity matrix */
int SceneNode::get_global_world_tansf(float *mat)
{
	Matrix mat_obj;
	if(parent!=NULL){
		parent->get_global_world_tansf(mat);
		}
	mat_obj.mul_mat(mat,world_trans_mat,mat);
	return 0;
}
/*
 * Set new transformation matrix by copying values
 */
int SceneNode::set_local_transf(float *mat)
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
int SceneNode::update_local_transf(float *mat)
{
	Matrix mat_obj;
	mat_obj.mul_mat(local_trans_mat,mat,local_trans_mat);
	return 0;
}

 /*
 * Return value for transformation matrix
 */

int SceneNode::get_local_transf(float *mat)
{
   memcpy(mat,local_trans_mat,16*sizeof(local_trans_mat[0]));
   return 0;
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


SceneNode * SceneNode::get_scenenode(int id)
{
	SceneNode *returned = NULL;
	if(id==scene_id){
		return this;
		}
	for( std::vector<SceneNode *>::iterator iter = child.begin(); iter != child.end(); ++iter ){
		returned = (*iter)->get_scenenode(id);
		if(returned!=NULL){
			return returned;
			}
	}
	return NULL;
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
		//glEnable(GL_COLOR_MATERIAL);	// specify object color
		glColor3f(1.0, 0.1, 0.1);		// redish
		glPushMatrix();
		glMultMatrixf(world_trans_mat);
		glStencilFunc(GL_ALWAYS,scene_id,-1);
	//This can be special node without model
	if(model!=NULL){
		glPushMatrix();
		glMultMatrixf(local_trans_mat);
		model->display();
		glPopMatrix();
		}
	// Display all child
	for( std::vector<SceneNode *>::iterator iter = child.begin(); iter != child.end(); ++iter ){
		(*iter)->display();
	}
	glPopMatrix();
	return 0;
}