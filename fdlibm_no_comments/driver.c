// fdlibm_driver.c
#include <stdio.h>
#include <math.h>

int main(void) {
    volatile double x = 1.5;
    volatile double y;

    y = acos(x);
    y = asin(x / 2);
    y = atan(x);
    y = cos(x);
    y = sin(x);

    return (int)y;
}
