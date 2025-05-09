#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define WINDOWS
#elif defined(__linux__)
    #define LINUX
    #define POSIX
#else
    #define UNKNOWN_OS
#endif
