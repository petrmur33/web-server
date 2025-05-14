#pragma once

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "utils.h"

#define REQUEST_LEN 10000
#define HEADER_LEN 100
#define TIME_LEN 100
#define CONTENT_LENGTH_INTEGER_LEN 15
#define FILE_PATH_LEN 1000
#define VERSION_LEN 20

#define HEADERS_DICT_SIZE 10000

typedef enum
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    PATCH,
    TRACE,
    CONNECT
} request_types;

typedef struct __buffer request_buffer;

typedef struct
{
    request_types type;
    char file_path[FILE_PATH_LEN];
    char version[VERSION_LEN];
    struct __dict headers;
    struct __buffer content;
} request;

enum __content_types {
    CT_PLAIN,
    CT_HTML,
    CT_CSS,
    CT_JS,
    CT_JSON,
    CT_JPEG,
    CT_XML,
    CT_PNG,
    CT_GIF,
    CT_SVG,
    CT_MPEG,
    CT_WAV,
    CT_MP4,
    CT_MSVIDEO,
    CT_OCTET_STREAM,
    CT_PDF,
    CT_ZIP,
    CT_TAR,
    CT_ICON
};
typedef enum __content_types content_types;

#define SERVER_NAME "petrmur-webserver"

#define HTTP_VERSION "HTTP/1.1 %i"

// const char* HTTP_VERSION_SUPPORT = { "1.1" };

request_buffer create_request(struct __buffer *content, content_types ct);
request_buffer add_http_content(char *req, struct __buffer *content);
size_t add_http_version(char *buf, size_t buf_size, int code);
size_t add_http_header(char *buf, size_t buf_size, char *header, char *value);
void init_request_struct(request *req, size_t content_size);
void free_request_struct(request *req);
void add_content_type(char *req, size_t req_size, content_types ct);