#include <iostream>
#include <cstring>
#include <matrix.h>
#include <math.h>

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}
/*
 * Get identity matrix of 4X4
 */
int Matrix::get_Imat(float *mat)
{
	memset(mat,0,MAT_ENT*sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
	return 0;
}

/*
 * Get translation matrix of 4X4
 */
int Matrix::get_Tmat(float x, float y, float z,float *mat)
{
	get_Imat(mat);
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	return 0;
}

/*
 * Get rotation matrix 4X4
 */
int Matrix::get_Rmat(float x,float y,float z,float angle_rad,float *mat)
{
	quat.CreateFromAxisAngle(x,y,z,angle_rad);
	quat.CreateMatrix(mat);
	return 0;
}

/*
 * Get Scaling matrix of 4X4
 */
int Matrix::get_Smat(float x, float y, float z,float *mat)
{
	get_Imat(mat);
	mat[0] = x;
	mat[5] = y;
	mat[10] = z;
	return 0;
}


float Matrix::get_angle(const Vector &vec1,const Vector &vec2)
{
	float angle = 0.0;
	float v1_mag = sqrt(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z * vec1.z);
	float v2_mag = sqrt(vec2.x*vec2.x + vec2.y*vec2.y + vec2.z * vec2.z);
	angle = acos(sqrt(vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z)/(v1_mag*v2_mag));
	angle = angle * 180 / M_PI;
	log_D("Angle: "<<angle);
	return angle;
}
/*
 * MAT_C = MAT_A * MAT_B
 */
int Matrix::mul_mat(float *mata,float *matb,float *matc)
{
	int n = 4;
	float ans[MAT_ENT];
	int i,j,k,sum=0;
	for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        for (k = 0; k < n; k++) {
          sum = sum + mata[i*n+k]*matb[k*n+j];
        }
        ans[i*n+j] = sum;
        sum = 0;
      }
    }
    memcpy(matc,ans,MAT_ENT*sizeof(ans[0]));
    return 0;
}