#include "parser.h"

void init_context(__context *context) {
    context->first_line   = true;
    context->new_line     = true;
    context->request_type = false;
    context->file_path    = false;
    context->http_version = false;
    context->key          = false;
    context->value        = false;
    context->content      = false;
}

// Reads text until stop_char meeted or stop is reached (exclusive)
// Returns pointer that points at stop_char
// Fully ignores '\n'
size_t parse_next_word(char *buf, size_t buf_size, const char *text, size_t start, size_t stop, char stop_char) {
    size_t buffer_pointer = 0;
    size_t text_pointer = start;
    while (buffer_pointer < buf_size - 1 && text[text_pointer] != stop_char && text_pointer < stop) {
        if (text[text_pointer] != '\n')
            buf[buffer_pointer++] = text[text_pointer];
        if (++text_pointer >= stop)
            break;
    }
    
    buf[buffer_pointer] = 0;
    return text_pointer;
}

// Returns 0 if all is good
parser_error parse_http_request(const char *request_text, size_t request_text_size, request *out) {
    char buffer1[TMP_BUFFER_SIZE];
    char buffer2[TMP_BUFFER_SIZE];
    buffer1[TMP_BUFFER_SIZE - 1] = 0; // For security reasons
    buffer2[TMP_BUFFER_SIZE - 1] = 0; // For security reasons
    size_t request_text_pointer = 0;
    
    __context context;
    init_context(&context);

    // Request type reading
    request_text_pointer = parse_next_word(buffer1, TMP_BUFFER_SIZE - 1, request_text, request_text_pointer, request_text_size, ' ');
    if (!strcmp("GET", buffer1))
        out->type = GET;
    else if (!strcmp("POST", buffer1))
        out->type = POST;
    else if (!strcmp("PUT", buffer1))
        out->type = PUT;
    else if (!strcmp("DELETE", buffer1))
        out->type = DELETE;
    else if (!strcmp("HEAD", buffer1))
        out->type = HEAD;
    else if (!strcmp("OPTIONS", buffer1))
        out->type = OPTIONS;
    else if (!strcmp("PATCH", buffer1))
        out->type = PATCH;
    else if (!strcmp("TRACE", buffer1))
        out->type = TRACE;
    else if (!strcmp("CONNECT", buffer1))
        out->type = CONNECT;
    else {
        return PE_REQ_TYPE;
    }
    context.request_type = true;
    context.new_line = false;
    
    // File path reading
    out->file_path[FILE_PATH_LEN - 1] = 0;
    request_text_pointer = parse_next_word(out->file_path, FILE_PATH_LEN - 1, request_text, request_text_pointer + 1, request_text_size, ' ');
    if (!validate_file_path(out->file_path) || request_text_pointer >= request_text_size)
        return PE_FILE_TYPE;
    context.file_path = true;
    
    // Version reading
    out->version[VERSION_LEN - 1] = 0;
    request_text_pointer = parse_next_word(out->version, VERSION_LEN - 1, request_text, request_text_pointer + 1, request_text_size, '\r');
    if (!validate_version(out->version) || request_text_pointer >= request_text_size)
        return PE_VERSION;
    if (request_text[++request_text_pointer] != '\n')
        return PE_VERSION;
    context.http_version = true;
    context.first_line = false;
    context.new_line = true;
    
    // Headers reading
    // TODO: headers in http are case insensitive, this parser ignores it
    while (1) {
        // Check if headers are done
        // TODO: this check may skip last 2 characters that can be bad
        if (request_text_size - request_text_pointer - 1 <= 2) {
            request_text_pointer = request_text_size - 1;
            break;
        }
        if (request_text[request_text_pointer + 1] == '\r' &&
            request_text[request_text_pointer + 2] == '\n') {
            request_text_pointer += 3;
            break;
        }
        
        // Key reading
        request_text_pointer = parse_next_word(buffer1, TMP_BUFFER_SIZE - 1, request_text, request_text_pointer + 1, request_text_size, ':');
        if (request_text[++request_text_pointer] != ' ')
            return PE_KEY;
        
        // Value reading
        request_text_pointer = parse_next_word(buffer2, TMP_BUFFER_SIZE, request_text, request_text_pointer + 1, request_text_size, '\r');
        if (request_text[++request_text_pointer] != '\n')
            return PE_VALUE;
        
        add_pair_to_dict(&out->headers, buffer1, buffer2);
    }
    
    // Content reading
    char *content_length_str = get_value_by_key(&out->headers, "Content-Length");
    if (content_length_str) {
        int content_length = atoi(content_length_str);
        if (content_length != 0 && content_length <= request_text_size - request_text_pointer)
            memcpy(out->content.buffer, request_text + request_text_pointer, content_length * sizeof(char));
        else
            return PE_CONTENT;
    }
    
    return PE_OK;
}

// TODO: Not implemented
bool validate_version(const char *version_str) {
    return true;
}

// TODO: Not implemented
bool validate_file_path(const char *file_path_str) {
    return true;
}