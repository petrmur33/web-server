#include "multithreading.h"

static void thread_error_handler(char *msg_format);

char error_message[ERROR_MESSAGE_LEN];

// You must write in msg_format at least one specifer for error code
static void thread_error_handler(char *msg_format) {
    int error_code = errno;
    
    snprintf(error_message, ERROR_MESSAGE_LEN, msg_format, error_code);
    strcat_s(error_message, ERROR_MESSAGE_LEN, "WARNING: error code are not implemented for POSIX!\n");
    error(error_message);
}

// Returns THREAD_STILL_ACTIVE if thread still active
thread_return thread_get_exit_code(thread thread) {
    void *ret;
    int err = pthread_tryjoin_np(thread, &ret);
    if (err == EBUSY)
	return THREAD_STILL_ACTIVE;
    
    if (err != 0)
	thread_error_handler("pthread_tryjoin_np function failure with code: %d\n");

    return ret;
}

thread thread_get_current_thread_id(void) {
    return pthread_self();
}

thread thread_create(thread_func func, thread_param param) {
    thread thread_handle;
    int error_code = pthread_create(
	&thread_handle,
	NULL,
	func,
	param
    );

    if (error_code)
	thread_error_handler("pthread_create function failure with code: %d\n");

    return thread_handle;
}

// TODO: Not implemented
void thread_suspend(thread _thread) {
    error("thread_suspend function is not implemented\n");
}

// TODO: Not implemented
void thread_resume(thread _thread) {
    error("thread_resume function is not implemented\n");
}

// NOTE: exit_code is not used in POSIX
void thread_terminate(thread _thread, thread_return exit_code) {
    int error_code = pthread_kill(_thread, SIGKILL);
    if (error_code) 
	thread_error_handler("pthread_kill function failure with code: %d\n");
}

void event_init(event *event_p) {
    int error_code = sem_init(event_p, 0, 0UL);

    if (error_code)
	thread_error_handler("sem_init function failure with code: %d\n");
}

void event_invoke(event *event_p) {
    int error_code = sem_post(event_p);
    if (error_code)
	thread_error_handler("sem_post function failure with code: %d\n");
}

void event_reset(event *event_p) {
    int error_code = sem_trywait(event_p);
    if (error_code)
	thread_error_handler("sem_trywait function failure with code: %d\n");
}

void event_wait(event *event_p) {
    int error_code_sem_wait = sem_wait(event_p);
    int error_code_sem_post = sem_post(event_p);
    if (error_code_sem_wait)
	thread_error_handler("sem_wait function failure with code: %d\n");
    if (error_code_sem_post)
	thread_error_handler("sem_post function failure with code: %d\n");
}
