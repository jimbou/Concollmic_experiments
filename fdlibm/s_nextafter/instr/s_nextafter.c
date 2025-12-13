





#include "fdlibm.h"
#include <stdio.h>

#ifdef __STDC__
	double nextafter(double x, double y)
#else
	double nextafter(x,y)
	double x,y;
#endif
{
	fprintf(stderr, "[s_nextafter.c] enter nextafter 1\n");
	int	hx,hy,ix,iy;
	unsigned lx,ly;

	hx = __HI(x);		
	lx = __LO(x);		
	hy = __HI(y);		
	ly = __LO(y);		
	ix = hx&0x7fffffff;		
	iy = hy&0x7fffffff;		
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 1\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 2\n");
	if(((ix>=0x7ff00000)&&((ix-0x7ff00000)|lx)!=0) ||    
	   ((iy>=0x7ff00000)&&((iy-0x7ff00000)|ly)!=0))      
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 3\n");
		return x+y;
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 3\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 2\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 4\n");
	if(x==y) 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 5\n");
		return x;
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 5\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 4\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 6\n");
	if((ix|lx)==0) 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 7\n");
		__HI(x) = hy&0x80000000;	
		__LO(x) = 1;
		y = x*x;
		if(y==x) 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 8\n");
			return y;
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 8\n");
		} 
		else 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 9\n");
			return x;
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 9\n");
		}
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 7\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 6\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 10\n");
	if(hx>=0) 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 11\n");
		if(hx>hy||((hx==hy)&&(lx>ly))) 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 12\n");
			if(lx==0) 
			{
				fprintf(stderr, "[s_nextafter.c] enter nextafter 13\n");
				hx -= 1;
				// fprintf(stderr, "[s_nextafter.c] exit nextafter 13\n");
			}
			fprintf(stderr, "[s_nextafter.c] enter nextafter 14\n");
			lx -= 1;
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 14\n");
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 12\n");
		} 
		else 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 15\n");
			lx += 1;
			if(lx==0) 
			{
				fprintf(stderr, "[s_nextafter.c] enter nextafter 16\n");
				hx += 1;
				// fprintf(stderr, "[s_nextafter.c] exit nextafter 16\n");
			}
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 15\n");
		}
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 11\n");
	} 
	else 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 17\n");
		if(hy>=0||hx>hy||((hx==hy)&&(lx>ly)))
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 18\n");
			if(lx==0) 
			{
				fprintf(stderr, "[s_nextafter.c] enter nextafter 19\n");
				hx -= 1;
				// fprintf(stderr, "[s_nextafter.c] exit nextafter 19\n");
			}
			fprintf(stderr, "[s_nextafter.c] enter nextafter 20\n");
			lx -= 1;
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 20\n");
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 18\n");
		} 
		else 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 21\n");
			lx += 1;
			if(lx==0) 
			{
				fprintf(stderr, "[s_nextafter.c] enter nextafter 22\n");
				hx += 1;
				// fprintf(stderr, "[s_nextafter.c] exit nextafter 22\n");
			}
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 21\n");
		}
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 17\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 10\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 23\n");
	hy = hx&0x7ff00000;
	if(hy>=0x7ff00000) 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 24\n");
		return x+x;
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 24\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 23\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 25\n");
	if(hy<0x00100000) 
	{
		fprintf(stderr, "[s_nextafter.c] enter nextafter 26\n");
		y = x*x;
		if(y!=x) 
		{
			fprintf(stderr, "[s_nextafter.c] enter nextafter 27\n");
			__HI(y) = hx; __LO(y) = lx;
			return y;
			// fprintf(stderr, "[s_nextafter.c] exit nextafter 27\n");
		}
		// fprintf(stderr, "[s_nextafter.c] exit nextafter 26\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 25\n");

	fprintf(stderr, "[s_nextafter.c] enter nextafter 28\n");
	__HI(x) = hx; __LO(x) = lx;
	return x;
	// fprintf(stderr, "[s_nextafter.c] exit nextafter 28\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "[s_nextafter.c] enter main 1\n");
	if (argc != 3) 
	{
		fprintf(stderr, "[s_nextafter.c] enter main 2\n");
		printf("Usage: %s <arg> <arg>\n", argv[0]);
		return 1;
		// fprintf(stderr, "[s_nextafter.c] exit main 2\n");
	}
	// fprintf(stderr, "[s_nextafter.c] exit main 1\n");

	fprintf(stderr, "[s_nextafter.c] enter main 3\n");
	double a0 = atof(argv[1]);
	double a1 = atof(argv[2]);
	// fprintf(stderr, "[s_nextafter.c] exit main 3\n");

	fprintf(stderr, "[s_nextafter.c] enter main 4\n");
	double r = nextafter(a0, a1);
	printf("%f\n", r);
	// fprintf(stderr, "[s_nextafter.c] exit main 4\n");

	fprintf(stderr, "[s_nextafter.c] enter main 5\n");
	return 0;
	// fprintf(stderr, "[s_nextafter.c] exit main 5\n");
}
// Total cost: 0.005536
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 83)]
// Total instrumented cost: 0.005536, input tokens: 7213, output tokens: 3262, cache read tokens: 0, cache write tokens: 0
