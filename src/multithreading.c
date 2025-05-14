#include "os_definition.h"

#if defined(WINDOWS)
    #include "multithreading_windows.c"
#elif defined(POSIX)
    #include "multithreading_posix.c"
#else
    #error "multithreading library doesn't support this operating system"
#endif
