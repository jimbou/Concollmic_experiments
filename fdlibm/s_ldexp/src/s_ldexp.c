



#include "fdlibm.h"
#include <errno.h>

#ifdef __STDC__
	double ldexp(double value, int exp)
#else
	double ldexp(value, exp)
	double value; int exp;
#endif
{
	if(!finite(value)||value==0.0) return value;
	value = scalbn(value,exp);
	if(!finite(value)||value==0.0) errno = ERANGE;
	return value;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s <arg> <arg>\n", argv[0]);
        return 1;
    }

    double a0 = atof(argv[1]);
    int a1 = atoi(argv[2]);

    double r = ldexp(a0, a1);
    printf("%f\n", r);

    return 0;
}
