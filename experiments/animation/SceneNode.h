
#ifndef _H_SCENE_NODE
#define _H_SCENE_NODE
#include <iostream>
#include <vector>
#include <algorithm>
#include <datatypes.h>

using namespace std;
class SceneNode	{
	public:
		SceneNode();
		~SceneNode();
		int add_child(SceneNode *node);
		/*
		 * mat is pointer to array having 16 consecutive floats
		 * set_transf copies data from mat to transformation matrix
		 * update_transf, updates transformation matrix
		 */
		int set_transf(float *mat);
		int update_transf(float *mat);
		/*
		 * Detach method will detach itself from parent(parent=NULL)
		 * Detach method will call parent's detach_child and detach
		 * it from parent side also
		 */
		int detach();
		int detach_child(SceneNode *child);
		int display();

	protected:
		float *local_trans_mat;
		float *world_trans_mat;
		//Model *model;
		SceneNode *parent;
		Vector *modelscale;
		std :: vector < SceneNode *> child;
	};

#endif