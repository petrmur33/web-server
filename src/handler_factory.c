// TODO: make single creation protection for this fabric

#include "handler_factory.h"

char error_message[ERROR_MESSAGE_LEN];
thread handlers_pool[MAX_HANDLERS];

event new_connection_event;
tcp_socket new_connection_socket;
void (*new_connection_handler_func)(unified_socket*);

static int get_empty_handler(void);
static int remove_complete_handlers(void);

void new_connection_invoke(tcp_socket s, void *(*handler_func)(unified_socket)) {
    new_connection_socket = s;
    new_connection_handler_func = handler_func;
    event_invoke(&new_connection_event);
}

// Returns NULL if error
thread create_handler(thread_func handler_func, tcp_socket s) {
    return thread_create(handler_func, (void *) s.socket);
}

// Returns number of new free handlers
static int remove_complete_handlers(void) {
    int counter = 0;
    for (int i = 0; i < MAX_HANDLERS; i++) {
        thread_return exit_code = thread_get_exit_code(handlers_pool[i]);
        if (exit_code != THREAD_STILL_ACTIVE) {
            handlers_pool[i] = NULL;
            counter++;
        }
    }
    
    return counter;
}

// Returns index to free place or -1 if handlers hasn't free space
static int get_empty_handler(void) {
    do
	for (int i = 0; i < MAX_HANDLERS; i++)
	    if (handlers_pool[i] == NULL) 
		return i;
    while (remove_complete_handlers() != 0);
    
    return -1;
}

thread_return main_factory(thread_param args) {
    event_init(&new_connection_event);
    memset(handlers_pool, NULL, sizeof(NULL)); // init handlers array
    
    while (1) {
	event_wait(&new_connection_event);
        event_reset(&new_connection_event);
        
        thread_func handler_func = new_connection_handler_func;
        tcp_socket s = new_connection_socket;
        
        int free_space_index = get_empty_handler();
        if (free_space_index == -1)
            tcp_socket_close(&s);
        else {
            thread handler = create_handler(handler_func, s);
            handlers_pool[free_space_index] = handler;
        }
    }
    
    return 0;
}

// Returns factory's thread
thread create_factory(void) {
    return thread_create(main_factory, NULL);
}
