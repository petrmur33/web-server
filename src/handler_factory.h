#pragma once

#include "tcp_socket.h"
#include "utils.h"
#include "multithreading.h"

#define MAX_HANDLERS 50

thread create_factory(void);
thread_return main_factory(thread_param args);
thread create_handler(thread_func handler_func, tcp_socket s);
void new_connection_invoke(tcp_socket s, void *(*handler_func)(unified_socket));
