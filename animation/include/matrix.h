#ifndef _H_MATRIX
#define _H_MATRIX

#define MAT_ENT 16
/* Matrix utility for opengl */
class Matrix{
	public:
		Matrix();
		~Matrix();
		int get_Imat(float *mat);
		int get_Tmat(float x, float y, float z,float *mat);
		int get_Rmat(float *mat);
		int get_Smat(float x, float y, float z,float *mat);
		int mul_mat(float *mata,float *matb,float *matc);
};

#endif