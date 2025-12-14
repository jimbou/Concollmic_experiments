
#include <math.h>

/* Try to override _LIB_VERSION if it's a global variable */
#ifdef _LIB_VERSION
int _LIB_VERSION = 1;  /* Non-IEEE value */
#endif
