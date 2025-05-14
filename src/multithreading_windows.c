#error "multithreading_windows.c not implemented yet" // TODO: replace types to unified typings
#include "multithreading.h"

static void thread_error_handler(char *msg_format);

char error_message[ERROR_MESSAGE_LEN];

// You must write in msg_format at least one specifer for error code
static void thread_error_handler(char *msg_format) {
    DWORD error_code = GetLastError();
    
    sprintf_s(error_message, ERROR_MESSAGE_LEN, msg_format, error_code);
    error(error_message);
}

// Returns THREAD_STILL_ACTIVE if thread still active
thread_return thread_get_exit_code(thread _thread) {
    DWORD exit_code;
    BOOL result = GetExitCodeThread(_thread, &exit_code);
    if (!result)
        thread_error_handler("GetExitCodeThread function failure with code: %lu\n");
    
    return exit_code;
}

thread thread_get_current_thread_id(void) {
    return GetCurrentThreadId();
}

thread thread_create(thread_func func, thread_param param) {
    DWORD thread_id;
    thread _thread = CreateThread(
        NULL,
        0,
        func,
        param,
        0,
        &thread_id
    );
    if (_thread == NULL)
        thread_error_handler("CreateThread function failure with code: %lu\n");
    return _thread;
}

void thread_suspend(thread _thread) {
    error("thread_suspend function is not implemented\n");
}

void thread_resume(thread _thread) {
    DWORD result = ResumeThread(_thread);
    if (result == -1)
        thread_error_handler("ResumeThread function failure with code: %lu\n");
}

void thread_terminate(thread _thread, thread_return exit_code) {
    bool result = TerminateThread(_thread, exit_code);
    if (!result)
        thread_error_handler("TerminateThread function failure with code: %lu\n");
}

void event_init(event *event_p) {
    event hEvent = CreateEventA(
        NULL,
        FALSE,
        TRUE,
        NULL
    );
    
    if (hEvent == NULL)
        thread_error_handler("CreateEventA function failure with code: %lu\n");

    *event_p = hEvent;
}

void event_invoke(event *event_p) {
    BOOL result = SetEvent(*event_p);
    if (!result)
        thread_error_handler("SetEvent function failure with code: %lu\n");
}

void event_reset(event *event_p) {
    BOOL result = ResetEvent(*event_p);
    if (!result)
        thread_error_handler("ResetEvent function failure with code: %lu\n");
}

void event_wait(event *event_p) {
    DWORD error_code = WaitForSingleObject(*event_p, INFINITE);
    if (error_code == WAIT_FAILED)
        thread_error_handler("WaitForSingleObject function failure with code: %lu\n");
}
