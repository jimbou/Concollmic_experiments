



#include "fdlibm.h"
#include <errno.h>

#ifndef _USE_WRITE
#include <stdio.h>			
#define	WRITE2(u,v)	fputs(u, stderr)
#else	
#include <unistd.h>			
#define	WRITE2(u,v)	write(2, u, v)
#undef fflush
#endif	

static double zero = 0.0;	




#ifdef __STDC__
	double __kernel_standard(double x, double y, int type) 
#else
	double __kernel_standard(x,y,type) 
	double x,y; int type;
#endif
{
	fprintf(stderr, "[k_standard.c] enter __kernel_standard 1\n");
	struct exception exc;
#ifndef HUGE_VAL	 
#define HUGE_VAL inf
	double inf = 0.0;

	__HI(inf) = 0x7ff00000;	
#endif

#ifdef _USE_WRITE
	(void) fflush(stdout);
#endif
	exc.arg1 = x;
	exc.arg2 = y;
	// fprintf(stderr, "[k_standard.c] exit __kernel_standard 1\n");
	switch(type) {
	    case 1:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 2\n");
		exc.type = DOMAIN;
		exc.name = "acos";
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if(_LIB_VERSION == _SVID_) {
		    (void) WRITE2("acos: DOMAIN error\n", 19);
		  }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 2\n");
	    case 2:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 3\n");
		exc.type = DOMAIN;
		exc.name = "asin";
		exc.retval = zero;
		if(_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if(_LIB_VERSION == _SVID_) {
		    	(void) WRITE2("asin: DOMAIN error\n", 19);
		  }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 3\n");
	    case 3:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 4\n");
		exc.arg1 = y;
		exc.arg2 = x;
		exc.type = DOMAIN;
		exc.name = "atan2";
		exc.retval = zero;
		if(_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if(_LIB_VERSION == _SVID_) {
			(void) WRITE2("atan2: DOMAIN error\n", 20);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 4\n");
	    case 4:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 5\n");
		exc.type = OVERFLOW;
		exc.name = "hypot";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = HUGE;
		else
		  exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 5\n");
	    case 5:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 6\n");
		exc.type = OVERFLOW;
		exc.name = "cosh";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = HUGE;
		else
		  exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 6\n");
	    case 6:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 7\n");
		exc.type = OVERFLOW;
		exc.name = "exp";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = HUGE;
		else
		  exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 7\n");
	    case 7:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 8\n");
		exc.type = UNDERFLOW;
		exc.name = "exp";
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 8\n");
	    case 8:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 9\n");
		exc.type = DOMAIN;	
		exc.name = "y0";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("y0: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 9\n");
	    case 9:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 10\n");
		exc.type = DOMAIN;
		exc.name = "y0";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("y0: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 10\n");
	    case 10:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 11\n");
		exc.type = DOMAIN;	
		exc.name = "y1";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("y1: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 11\n");
	    case 11:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 12\n");
		exc.type = DOMAIN;
		exc.name = "y1";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("y1: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 12\n");
	    case 12:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 13\n");
		exc.type = DOMAIN;	
		exc.name = "yn";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("yn: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 13\n");
	    case 13:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 14\n");
		exc.type = DOMAIN;
		exc.name = "yn";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("yn: DOMAIN error\n", 17);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 14\n");
	    case 14:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 15\n");
		exc.type = OVERFLOW;
		exc.name = "lgamma";
                if (_LIB_VERSION == _SVID_)
                  exc.retval = HUGE;
                else
                  exc.retval = HUGE_VAL;
                if (_LIB_VERSION == _POSIX_)
			errno = ERANGE;
                else if (!matherr(&exc)) {
                        errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 15\n");
	    case 15:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 16\n");
		exc.type = SING;
		exc.name = "lgamma";
                if (_LIB_VERSION == _SVID_)
                  exc.retval = HUGE;
                else
                  exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("lgamma: SING error\n", 19);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 16\n");
	    case 16:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 17\n");
		exc.type = SING;
		exc.name = "log";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("log: SING error\n", 16);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 17\n");
	    case 17:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 18\n");
		exc.type = DOMAIN;
		exc.name = "log";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("log: DOMAIN error\n", 18);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 18\n");
	    case 18:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 19\n");
		exc.type = SING;
		exc.name = "log10";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("log10: SING error\n", 18);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 19\n");
	    case 19:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 20\n");
		exc.type = DOMAIN;
		exc.name = "log10";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = -HUGE;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("log10: DOMAIN error\n", 20);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 20\n");
	    case 20:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 21\n");
		exc.type = DOMAIN;
		exc.name = "pow";
		exc.retval = zero;
		if (_LIB_VERSION != _SVID_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			(void) WRITE2("pow(0,0): DOMAIN error\n", 23);
			errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 21\n");
	    case 21:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 22\n");
		exc.type = OVERFLOW;
		exc.name = "pow";
		if (_LIB_VERSION == _SVID_) {
		  exc.retval = HUGE;
		  y *= 0.5;
		  if(x<zero&&rint(y)!=y) exc.retval = -HUGE;
		} else {
		  exc.retval = HUGE_VAL;
		  y *= 0.5;
		  if(x<zero&&rint(y)!=y) exc.retval = -HUGE_VAL;
		}
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 22\n");
	    case 22:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 23\n");
		exc.type = UNDERFLOW;
		exc.name = "pow";
		exc.retval =  zero;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 23\n");
	    case 23:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 24\n");
		exc.type = DOMAIN;
		exc.name = "pow";
		if (_LIB_VERSION == _SVID_) 
		  exc.retval = zero;
		else
		  exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("pow(0,neg): DOMAIN error\n", 25);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 24\n");
	    case 24:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 25\n");
		exc.type = DOMAIN;
		exc.name = "pow";
		if (_LIB_VERSION == _SVID_) 
		    exc.retval = zero;
		else 
		    exc.retval = zero/zero;	
		if (_LIB_VERSION == _POSIX_) 
		   errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("neg**non-integral: DOMAIN error\n", 32);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 25\n");
	    case 25:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 26\n");
		exc.type = OVERFLOW;
		exc.name = "sinh";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = ( (x>zero) ? HUGE : -HUGE);
		else
		  exc.retval = ( (x>zero) ? HUGE_VAL : -HUGE_VAL);
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 26\n");
	    case 26:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 27\n");
		exc.type = DOMAIN;
		exc.name = "sqrt";
		if (_LIB_VERSION == _SVID_)
		  exc.retval = zero;
		else
		  exc.retval = zero/zero;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("sqrt: DOMAIN error\n", 19);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 27\n");
            case 27:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 28\n");
                exc.type = DOMAIN;
                exc.name = "fmod";
                if (_LIB_VERSION == _SVID_)
                    exc.retval = x;
		else
		    exc.retval = zero/zero;
                if (_LIB_VERSION == _POSIX_)
                  errno = EDOM;
                else if (!matherr(&exc)) {
                  if (_LIB_VERSION == _SVID_) {
                    (void) WRITE2("fmod:  DOMAIN error\n", 20);
                  }
                  errno = EDOM;
                }
                break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 28\n");
            case 28:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 29\n");
                exc.type = DOMAIN;
                exc.name = "remainder";
                exc.retval = zero/zero;
                if (_LIB_VERSION == _POSIX_)
                  errno = EDOM;
                else if (!matherr(&exc)) {
                  if (_LIB_VERSION == _SVID_) {
                    (void) WRITE2("remainder: DOMAIN error\n", 24);
                  }
                  errno = EDOM;
                }
                break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 29\n");
            case 29:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 30\n");
                exc.type = DOMAIN;
                exc.name = "acosh";
                exc.retval = zero/zero;
                if (_LIB_VERSION == _POSIX_)
                  errno = EDOM;
                else if (!matherr(&exc)) {
                  if (_LIB_VERSION == _SVID_) {
                    (void) WRITE2("acosh: DOMAIN error\n", 20);
                  }
                  errno = EDOM;
                }
                break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 30\n");
            case 30:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 31\n");
                exc.type = DOMAIN;
                exc.name = "atanh";
                exc.retval = zero/zero;
                if (_LIB_VERSION == _POSIX_)
                  errno = EDOM;
                else if (!matherr(&exc)) {
                  if (_LIB_VERSION == _SVID_) {
                    (void) WRITE2("atanh: DOMAIN error\n", 20);
                  }
                  errno = EDOM;
                }
                break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 31\n");
            case 31:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 32\n");
                exc.type = SING;
                exc.name = "atanh";
		exc.retval = x/zero;	
                if (_LIB_VERSION == _POSIX_)
                  errno = EDOM;
                else if (!matherr(&exc)) {
                  if (_LIB_VERSION == _SVID_) {
                    (void) WRITE2("atanh: SING error\n", 18);
                  }
                  errno = EDOM;
                }
                break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 32\n");
	    case 32:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 33\n");
		exc.type = OVERFLOW;
		exc.name = "scalb";
		exc.retval = x > zero ? HUGE_VAL : -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 33\n");
	    case 33:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 34\n");
		exc.type = UNDERFLOW;
		exc.name = "scalb";
		exc.retval = copysign(zero,x);
		if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
		else if (!matherr(&exc)) {
			errno = ERANGE;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 34\n");
	    case 34:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 35\n");
                exc.type = TLOSS;
                exc.name = "j0";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 35\n");
	    case 35:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 36\n");
                exc.type = TLOSS;
                exc.name = "y0";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 36\n");
	    case 36:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 37\n");
                exc.type = TLOSS;
                exc.name = "j1";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 37\n");
	    case 37:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 38\n");
                exc.type = TLOSS;
                exc.name = "y1";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 38\n");
	    case 38:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 39\n");
                exc.type = TLOSS;
                exc.name = "jn";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 39\n");
	    case 39:
                fprintf(stderr, "[k_standard.c] enter __kernel_standard 40\n");
                exc.type = TLOSS;
                exc.name = "yn";
                exc.retval = zero;
                if (_LIB_VERSION == _POSIX_)
                        errno = ERANGE;
                else if (!matherr(&exc)) {
                        if (_LIB_VERSION == _SVID_) {
                                (void) WRITE2(exc.name, 2);
                                (void) WRITE2(": TLOSS error\n", 14);
                        }
                        errno = ERANGE;
                }        
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 40\n");
	    case 40:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 41\n");
		exc.type = OVERFLOW;
		exc.name = "gamma";
                if (_LIB_VERSION == _SVID_)
                  exc.retval = HUGE;
                else
                  exc.retval = HUGE_VAL;
                if (_LIB_VERSION == _POSIX_)
		  errno = ERANGE;
                else if (!matherr(&exc)) {
                  errno = ERANGE;
                }
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 41\n");
	    case 41:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 42\n");
		exc.type = SING;
		exc.name = "gamma";
                if (_LIB_VERSION == _SVID_)
                  exc.retval = HUGE;
                else
                  exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
		  errno = EDOM;
		else if (!matherr(&exc)) {
		  if (_LIB_VERSION == _SVID_) {
			(void) WRITE2("gamma: SING error\n", 18);
		      }
		  errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 42\n");
	    case 42:
		fprintf(stderr, "[k_standard.c] enter __kernel_standard 43\n");
		exc.type = DOMAIN;
		exc.name = "pow";
		exc.retval = x;
		if (_LIB_VERSION == _IEEE_ ||
		    _LIB_VERSION == _POSIX_) exc.retval = 1.0;
		else if (!matherr(&exc)) {
			errno = EDOM;
		}
		break;
		// fprintf(stderr, "[k_standard.c] exit __kernel_standard 43\n");
	}
	fprintf(stderr, "[k_standard.c] enter __kernel_standard 44\n");
	return exc.retval; 
	// fprintf(stderr, "[k_standard.c] exit __kernel_standard 44\n");
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	fprintf(stderr, "\n");
    if (argc != 4) {
        fprintf(stderr, "[k_standard.c] enter main 2\n");
        printf("Usage: %s <arg> <arg> <arg>\n", argv[0]);
        return 1;
        // fprintf(stderr, "[k_standard.c] exit main 2\n");
    }

    fprintf(stderr, "[k_standard.c] enter main 3\n");
    double a0 = atof(argv[1]);
    double a1 = atof(argv[2]);
    int a2 = atoi(argv[3]);
    // fprintf(stderr, "[k_standard.c] exit main 3\n");

    fprintf(stderr, "[k_standard.c] enter main 4\n");
    double r = __kernel_standard(a0, a1, a2);
    printf("%f\n", r);

    return 0;
    // fprintf(stderr, "[k_standard.c] exit main 4\n");
}
// Total cost: 0.009324
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 698)]
// Total instrumented cost: 0.009324, input tokens: 7338, output tokens: 6675, cache read tokens: 0, cache write tokens: 0
