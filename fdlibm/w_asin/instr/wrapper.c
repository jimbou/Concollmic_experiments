
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <math.h>

/* Define the possible _LIB_VERSION values if they're not already defined */
#ifndef _IEEE_
#define _IEEE_ 0
#endif
#ifndef _SVID_
#define _SVID_ 1
#endif

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }
    
    /* Try to find and modify _LIB_VERSION using dlsym */
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle) {
        int *lib_version_ptr = (int *)dlsym(handle, "_LIB_VERSION");
        if (lib_version_ptr) {
            fprintf(stderr, "Found _LIB_VERSION at %p, original value: %d\n", lib_version_ptr, *lib_version_ptr);
            *lib_version_ptr = _SVID_;  /* Set to non-IEEE value */
            fprintf(stderr, "Set _LIB_VERSION to %d (_SVID_)\n", *lib_version_ptr);
        } else {
            fprintf(stderr, "Could not find _LIB_VERSION symbol\n");
        }
        dlclose(handle);
    }
    
    /* Execute the original program */
    char cmd[1024] = {0};
    for (int i = 1; i < argc; i++) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    
    fprintf(stderr, "Executing: %s\n", cmd);
    return system(cmd);
}
