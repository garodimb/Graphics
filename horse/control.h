struct Vec3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	};
	
struct Point {
	GLfloat x;
	GLfloat y;
};

#define KEY_ENTER 13 //Switch Mode Key
#define KEY_ESC 27 //Exit Application Key
#define ZOOMIN 43
#define ZOOMOUT 45
#define KEY_L 108 // Left Rotation
#define KEY_R 114 // Right Rotation
#define KEY_H 104
#define KEY_V 118
#define KEY_D 100
#define KEY_x 120
#define KEY_X 88
#define KEY_y 121
#define KEY_Y 89
#define KEY_z 122
#define KEY_Z 90
#define KEY_S 83
#define KEY_s 115

#define log_I(LOG_MESSAGE) std::cout<<"[INFO] "<< LOG_MESSAGE <<std::endl

#ifdef DEBUG
#define log_D(LOG_MESSAGE) std::cout<<"["<<__FILE__<<":"<<__LINE__<<"][DEBUG] "<< LOG_MESSAGE <<std::endl
#else
#define log_D(LOG_MESSAGE) 
#endif
#define log_E(LOG_MESSAGE) std::cout<<"["<<__FILE__<<":"<<__LINE__<<"][ERROR] "<< LOG_MESSAGE <<std::endl
