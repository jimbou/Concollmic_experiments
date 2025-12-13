#pragma ident "@(#)k_tan.c 1.5 04/04/22 SMI"






#include "fdlibm.h"
#include <stdio.h>

static const double xxx[] = {
		 3.33333333333334091986e-01,	
		 1.33333333333201242699e-01,	
		 5.39682539762260521377e-02,	
		 2.18694882948595424599e-02,	
		 8.86323982359930005737e-03,	
		 3.59207910759131235356e-03,	
		 1.45620945432529025516e-03,	
		 5.88041240820264096874e-04,	
		 2.46463134818469906812e-04,	
		 7.81794442939557092300e-05,	
		 7.14072491382608190305e-05,	
		-1.85586374855275456654e-05,	
		 2.59073051863633712884e-05,	
	 1.00000000000000000000e+00,	
	 7.85398163397448278999e-01,	
	 3.06161699786838301793e-17	
};
#define	one	xxx[13]
#define	pio4	xxx[14]
#define	pio4lo	xxx[15]
#define	T	xxx


double
__kernel_tan(double x, double y, int iy) {
	fprintf(stderr, "[k_tan.c] enter __kernel_tan 1\n");
	double z, r, v, w, s;
	int ix, hx;

	hx = __HI(x);		
	ix = hx & 0x7fffffff;			
	if (ix < 0x3e300000) {			
		if ((int) x == 0) {		
			if (((ix | __LO(x)) | (iy + 1)) == 0)
			{
				fprintf(stderr, "[k_tan.c] enter __kernel_tan 2\n");
				return one / fabs(x);
				// fprintf(stderr, "[k_tan.c] exit __kernel_tan 2\n");
			}
			else {
				fprintf(stderr, "[k_tan.c] enter __kernel_tan 3\n");
				if (iy == 1)
				{
					fprintf(stderr, "[k_tan.c] enter __kernel_tan 4\n");
					return x;
					// fprintf(stderr, "[k_tan.c] exit __kernel_tan 4\n");
				}
				else {	
					fprintf(stderr, "[k_tan.c] enter __kernel_tan 5\n");
					double a, t;

					z = w = x + y;
					__LO(z) = 0;
					v = y - (z - x);
					t = a = -one / w;
					__LO(t) = 0;
					s = one + t * z;
					return t + a * (s + t * v);
					// fprintf(stderr, "[k_tan.c] exit __kernel_tan 5\n");
				}
				// fprintf(stderr, "[k_tan.c] exit __kernel_tan 3\n");
			}
		}
	}
	// fprintf(stderr, "[k_tan.c] exit __kernel_tan 1\n");
	if (ix >= 0x3FE59428) {	
		fprintf(stderr, "[k_tan.c] enter __kernel_tan 6\n");
		if (hx < 0) {
			fprintf(stderr, "[k_tan.c] enter __kernel_tan 7\n");
			x = -x;
			y = -y;
			// fprintf(stderr, "[k_tan.c] exit __kernel_tan 7\n");
		}
		z = pio4 - x;
		w = pio4lo - y;
		x = z + w;
		y = 0.0;
		// fprintf(stderr, "[k_tan.c] exit __kernel_tan 6\n");
	}
	fprintf(stderr, "[k_tan.c] enter __kernel_tan 8\n");
	z = x * x;
	w = z * z;
	
	r = T[1] + w * (T[3] + w * (T[5] + w * (T[7] + w * (T[9] +
		w * T[11]))));
	v = z * (T[2] + w * (T[4] + w * (T[6] + w * (T[8] + w * (T[10] +
		w * T[12])))));
	s = z * x;
	r = y + z * (s * (r + v) + y);
	r += T[0] * s;
	w = x + r;
	if (ix >= 0x3FE59428) {
		fprintf(stderr, "[k_tan.c] enter __kernel_tan 9\n");
		v = (double) iy;
		return (double) (1 - ((hx >> 30) & 2)) *
			(v - 2.0 * (x - (w * w / (w + v) - r)));
		// fprintf(stderr, "[k_tan.c] exit __kernel_tan 9\n");
	}
	// fprintf(stderr, "[k_tan.c] exit __kernel_tan 8\n");
	if (iy == 1)
	{
		fprintf(stderr, "[k_tan.c] enter __kernel_tan 10\n");
		return w;
		// fprintf(stderr, "[k_tan.c] exit __kernel_tan 10\n");
	}
	else {
		fprintf(stderr, "[k_tan.c] enter __kernel_tan 11\n");
		
		
		double a, t;
		z = w;
		__LO(z) = 0;
		v = r - (z - x);	
		t = a = -1.0 / w;	
		__LO(t) = 0;
		s = 1.0 + t * z;
		return t + a * (s + t * v);
		// fprintf(stderr, "[k_tan.c] exit __kernel_tan 11\n");
	}
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[k_tan.c] enter main 1\n");
	if (argc != 4) {
		fprintf(stderr, "[k_tan.c] enter main 2\n");
		printf("Usage: %s <arg> <arg> <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[k_tan.c] exit main 2\n");
	}
	// fprintf(stderr, "[k_tan.c] exit main 1\n");

	fprintf(stderr, "[k_tan.c] enter main 3\n");
	double a0 = atof(argv[1]);
	double a1 = atof(argv[2]);
	int a2 = atoi(argv[3]);

	double r = __kernel_tan(a0, a1, a2);
	printf("%f\n", r);

	return 0;
	// fprintf(stderr, "[k_tan.c] exit main 3\n");
}
// Total cost: 0.002574
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 122)]
// Total instrumented cost: 0.002574, input tokens: 3203, output tokens: 1554, cache read tokens: 0, cache write tokens: 0
