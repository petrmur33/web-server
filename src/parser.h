#pragma once

#include "utils.h"
#include "request.h"

#include <stdbool.h>

#define TMP_BUFFER_SIZE 10000

// TODO: maybe it's not neccessary thing
typedef struct {
    bool first_line;
    bool request_type;
    bool file_path;
    bool http_version;
    bool key;
    bool value;
    bool content;
    bool new_line;
} __context;

typedef enum {
    PE_OK,
    PE_REQ_TYPE,
    PE_FILE_TYPE,
    PE_VERSION,
    PE_KEY,
    PE_VALUE,
    PE_CONTENT
} parser_error;

void init_context(__context *context);
size_t parse_next_word(char *buf, size_t buf_size, const char *text, size_t start, size_t stop, char stop_char);
parser_error parse_http_request(const char *request_text, size_t request_text_size, request *out);
bool validate_version(const char *version_str);
bool validate_file_path(const char *file_path_str);
