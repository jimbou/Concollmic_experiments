
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
    double x = 0.0;  // Use a non-positive value to trigger the target branch
    double result = y1(x);
    printf("y1(%f) = %f\n", x, result);
    return 0;
}
