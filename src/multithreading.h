#pragma once

#include "os_definition.h"
#include "utils.h"

#if defined(WINDOWS)
    #include <windows.h>
#elif defined(POSIX)
    #define _GNU_SOURCE
    #include <pthread.h>
    #include <semaphore.h>
    #include <errno.h>
    #include <limits.h>
    #include <signal.h>
#endif

#include <stdbool.h>

#if defined(WINDOWS)
    #define THREAD_STILL_ACTIVE STILL_ACTIVE
    typedef HANDLE thread;
    typedef DWORD thread_return; // TODO: for some reason return type must be DWORD WINAPI
    typedef LPVOID thread_param;

    typedef HANDLE event;
#elif defined(POSIX)
    #define THREAD_STILL_ACTIVE (void *) ULLONG_MAX
    typedef pthread_t thread;
    typedef void* thread_return;
    typedef void* thread_param;

    typedef sem_t event;
#endif
typedef thread_return (*thread_func)(thread_param);

thread_return thread_get_exit_code(thread _thread);
thread thread_get_current_thread_id(void);
thread thread_create(thread_func func, thread_param param);
void thread_suspend(thread _thread);
void thread_resume(thread _thread);
void thread_terminate(thread _thread, thread_return exit_code);
void event_init(event *event_p);
void event_invoke(event *event_p);
void event_reset(event *event_p);
void event_wait(event *event_p);
