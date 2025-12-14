
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *handle;
    int *lib_version;
    int *ieee, *svid, *xopen, *posix;
    char *error;

    // Open the math library
    handle = dlopen("libm.so.6", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error opening libm.so.6: %s\n", dlerror());
        return 1;
    }

    // Clear any existing error
    dlerror();

    // Try to find _LIB_VERSION symbol
    lib_version = (int *)dlsym(handle, "_LIB_VERSION");
    error = dlerror();
    if (error) {
        printf("Could not find _LIB_VERSION: %s\n", error);
    } else {
        printf("Found _LIB_VERSION at %p, value = %d\n", lib_version, *lib_version);
    }

    // Try to find the constants
    ieee = (int *)dlsym(handle, "_IEEE_");
    svid = (int *)dlsym(handle, "_SVID_");
    xopen = (int *)dlsym(handle, "_XOPEN_");
    posix = (int *)dlsym(handle, "_POSIX_");

    if (ieee) printf("_IEEE_ = %d\n", *ieee);
    if (svid) printf("_SVID_ = %d\n", *svid);
    if (xopen) printf("_XOPEN_ = %d\n", *xopen);
    if (posix) printf("_POSIX_ = %d\n", *posix);

    // If we found _LIB_VERSION, try to modify it
    if (lib_version && svid) {
        printf("Attempting to change _LIB_VERSION from %d to %d (_SVID_)\n", 
               *lib_version, *svid);
        *lib_version = *svid;
        printf("After modification: _LIB_VERSION = %d\n", *lib_version);
    }

    // Close the library
    dlclose(handle);
    return 0;
}
