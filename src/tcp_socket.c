#include "os_definition.h"

#if defined(POSIX)
    #include "tcp_socket_posix.c"
#elif defined(WINDOWS)
    #include "tcp_socket_windows.c"
#else
    #error "tcp_socket library doesn't support this operating system"
#endif
