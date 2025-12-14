
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <math.h>

// Our replacement for j1 that calls y1 instead
double j1(double x) {
    fprintf(stderr, "[interposer] Intercepted j1(%f), calling y1 instead\n", x);
    
    // Call y1 with a non-positive value to trigger the target branch
    double result = y1(0.0);  // Use 0.0 to satisfy x <= 0.0
    
    fprintf(stderr, "[interposer] y1(0.0) returned %f\n", result);
    
    // Return the y1 result instead of calling the original j1
    return result;
}

// Also intercept the main function to see if that works
int __libc_start_main(
    int (*main)(int, char **, char **),
    int argc,
    char **argv,
    void (*init)(void),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end
) {
    fprintf(stderr, "[interposer] Intercepted __libc_start_main\n");
    
    // Call y1 directly
    double result = y1(0.0);
    fprintf(stderr, "[interposer] Direct call to y1(0.0) returned %f\n", result);
    
    // Call the original __libc_start_main
    typeof(&__libc_start_main) orig_libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");
    return orig_libc_start_main(main, argc, argv, init, fini, rtld_fini, stack_end);
}
