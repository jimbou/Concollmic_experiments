





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	int finite(double x)
#else
	int finite(x)
	double x;
#endif
{
	int hx; 
	hx = __HI(x);
	return  (unsigned)((hx&0x7fffffff)-0x7ff00000)>>31;
}

#include <stdio.h>
#include <stdlib.h>

