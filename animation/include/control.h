
#define log_I(LOG_MESSAGE) std::cout<<"[INFO] "<< LOG_MESSAGE <<std::endl

#ifdef DEBUG
#define log_D(LOG_MESSAGE) std::cout<<"["<<__FILE__<<":"<<__LINE__<<"][DEBUG] "<< LOG_MESSAGE <<std::endl
#else
#define log_D(LOG_MESSAGE) 
#endif
#define log_E(LOG_MESSAGE) std::cout<<"["<<__FILE__<<":"<<__LINE__<<"][ERROR] "<< LOG_MESSAGE <<std::endl
