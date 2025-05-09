#include "request.h"

void add_content_type(char *req, size_t req_size, content_types ct) {
    char *value;
    switch (ct) {
        case CT_PLAIN:
            value = "text/plain";
            break;
        case CT_HTML:
            value = "text/html";
            break;
        case CT_CSS:
            value = "text/css";
            break;
        case CT_JS:
            value = "text/javascript";
            break;
        case CT_JSON:
            value = "application/json";
            break;
        case CT_JPEG:
            value = "image/jpeg";
            break;
        case CT_XML:
            value = "application/xml";
            break;
        case CT_PNG:
            value = "image/png";
            break;
        case CT_GIF:
            value = "image/gif";
            break;
        case CT_SVG:
            value = "image/svg+xml";
            break;
        case CT_MPEG:
            value = "audio/mpeg";
            break;
        case CT_WAV:
            value = "audio/wav";
            break;
        case CT_MP4:
            value = "video/mp4";
            break;
        case CT_MSVIDEO:
            value = "video/x-msvideo";
            break;
        case CT_OCTET_STREAM:
            value = "application/octet-stream";
            break;
        case CT_PDF:
            value = "application/pdf";
            break;
        case CT_ZIP:
            value = "application/zip";
            break;
        case CT_TAR:
            value = "application/x-tar";
            break;
        case CT_ICON:
            value = "image/x-icon";
            break;
        default:
            value = "unknown";
    }
    
    add_http_header(req, req_size, "Content-Type", value);
}

// content can be NULL
// this function adds entire content buffer at the bottom of request
request_buffer create_request(struct __buffer *content, content_types ct) {
    char request[REQUEST_LEN];
    request[0] = 0;

    char current_time[TIME_LEN];
    format_time(time(NULL), current_time, TIME_LEN);

    add_http_version(request, REQUEST_LEN, 200);

    add_http_header(request, REQUEST_LEN, "Date", current_time);
    add_http_header(request, REQUEST_LEN, "Server", SERVER_NAME);
    add_http_header(request, REQUEST_LEN, "Connection", "close");
    
    request_buffer completed_request;
    if (content != NULL) {
        add_content_type(request, REQUEST_LEN, ct);
        
        char content_length_string[CONTENT_LENGTH_INTEGER_LEN];
        snprintf(content_length_string, CONTENT_LENGTH_INTEGER_LEN, "%lu", content->size);
        add_http_header(request, REQUEST_LEN, "Content-Length", content_length_string);
        completed_request = add_http_content(request, content);
    }
    else
        completed_request = add_http_content(request, NULL);

    return completed_request;
}

// returns request_buffer that contains completed http request
// add entire content buffer at the end of request
// if content is NULL, then func only completes http request
request_buffer add_http_content(char *req, struct __buffer *content) {
    size_t request_wo_content_len = strlen(req) + 2; // With header ending
    size_t neccessary_size = request_wo_content_len;
    if (content != NULL)
        neccessary_size += content->size;

    request_buffer ret;
    init_buffer(&ret, neccessary_size);

    strcat_s(ret.buffer, ret.size, req);
    strcat_s(ret.buffer, ret.size, "\r\n");
    if (content != NULL)
        memcpy(ret.buffer + request_wo_content_len, content->buffer, content->size);
    
    return ret;
}

// returns number of bytes added
// returns 0 if buffer is small
size_t add_http_version(char *buf, size_t buf_size, int code) {
    char header[HEADER_LEN];
    size_t bytes_added = snprintf(header, HEADER_LEN, HTTP_VERSION, code) + 2;  // With CRLF at the end

    size_t necessary_size = bytes_added + strlen(buf) + 1; // With zero at the end
    if (buf_size < necessary_size) 
        return 0;

    strcat_s(buf, buf_size, header);
    strcat_s(buf, buf_size, "\r\n");
    
    return bytes_added;
}

// returns number of bytes added
// returns 0 if buffer is small
size_t add_http_header(char *buf, size_t buf_size, char *header, char *value) {
    size_t bytes_added = strlen(header) + strlen(value) + 4; // With separator and CRLF at the end
    size_t necessary_size = strlen(buf) + 1; // with zero at the end
    if (buf_size < necessary_size)
        return 0;
    // TODO: Maybe it's not neccessary to use secure string functions here because out of bounds situation is already checked.
    strcat_s(buf, buf_size, header);
    strcat_s(buf, buf_size, ": ");
    strcat_s(buf, buf_size, value);
    strcat_s(buf, buf_size, "\r\n");
    
    return bytes_added;
}

void init_request_struct(request *req, size_t content_size) {
    init_buffer(&req->content, content_size);
    init_dict(&req->headers, HEADERS_DICT_SIZE, HEADERS_DICT_SIZE);
}

void free_request_struct(request *req) {
    free_buffer(&req->content);
    free_dict(&req->headers);
}