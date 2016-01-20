/* Header file to define logging level
   INFO->All levels
   DEBUG->ERROR and DEBUG
   By default only error level is active
   */

#ifdef INFO
#define log_I(LOG_MESSAGE) printf("[%s:%d][INFO] %s\n",__FILE__,__LINE__,LOG_MESSAGE)
#define DEBUG
#else
#define log_I(LOG_MESSAGE) 
#endif

#ifdef DEBUG
#define log_D(LOG_MESSAGE) printf("[%s:%d][DEBUG] %s\n",__FILE__,__LINE__,LOG_MESSAGE)
#else
#define log_D(LOG_MESSAGE) 
#endif
#define log_E(LOG_MESSAGE) printf("[%s:%d][ERROR] %s\n",__FILE__,__LINE__,LOG_MESSAGE)
