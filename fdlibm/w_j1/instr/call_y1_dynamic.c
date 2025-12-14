
    #include <stdio.h>
    #include <stdlib.h>
    #include <dlfcn.h>
    #include <math.h>
    
    int main() {
        // Try to load the y1 function from the temp_executable
        void* handle = dlopen("./temp_executable", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "dlopen error: %s\n", dlerror());
            
            // Fall back to using the standard math library
            printf("Falling back to standard math library\n");
            double x = 0.0;  // Use a non-positive value to trigger the target branch
            double result = y1(x);
            printf("y1(%f) = %f\n", x, result);
            return 0;
        }
        
        // Try to get the y1 function
        double (*y1_func)(double) = dlsym(handle, "y1");
        if (!y1_func) {
            fprintf(stderr, "dlsym error: %s\n", dlerror());
            dlclose(handle);
            
            // Fall back to using the standard math library
            printf("Falling back to standard math library\n");
            double x = 0.0;  // Use a non-positive value to trigger the target branch
            double result = y1(x);
            printf("y1(%f) = %f\n", x, result);
            return 0;
        }
        
        // Call the y1 function with a non-positive value
        double x = 0.0;
        double result = y1_func(x);
        printf("y1(%f) = %f\n", x, result);
        
        dlclose(handle);
        return 0;
    }
    