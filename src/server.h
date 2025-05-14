#pragma once

#include "webpage.h"
#include "request.h"
#include "utils.h"
#include "webpages.h"
#include "parser.h"
#include "tcp_socket.h"
#include "handler_factory.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP_ADDRESS "127.0.0.1"
#define PORT 80

#define RECIEVE_BUFFER_LEN 1024

#define LOGGER_GLOBAL_PATH "./webserver.log"

#define PATH_LEN 1000
#define ROOT_DIR "./assets"

typedef enum {
    uninitialized,
    initialized,
    opened,
    closed
} server_status;

typedef struct __buffer recieve_buffer;

typedef struct
{
    tcp_socket socket;
    server_status status;
} server;

void send_static_html(tcp_socket *s, const char *html_path);
void send_welcome(tcp_socket *s);
bool request_handler(request *req, tcp_socket *s);
content_types guess_content_format_by_extension(const char *file_name);
void *connection_handler(unified_socket sock);

int create_server(server *s);
int run_server(server *s);
