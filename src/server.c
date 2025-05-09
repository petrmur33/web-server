#include "server.h"

bool tcp_sockets_initialized = false; // TODO: Why I do that. that used only three times

// Returns 1 if all is good
int create_server(server *s) {
    if (!tcp_sockets_initialized) {
        initialize_tcp_sockets();
        tcp_sockets_initialized = true;
    }
    
    if (logger_create_global(LOGGER_GLOBAL_PATH, true, LOGGER_DEBUG) == 1)
        return 0;

    tcp_socket_init(&(s->socket), IP_ADDRESS, PORT);
    tcp_socket_listen(&(s->socket), SOMAXCONN);
    s->status = initialized;
    
    return 1;
}

// Returns 1 if all is good
int run_server(server *s) {
    if (s->status != initialized)
        return 0;

    s->status = opened;

    thread factory = create_factory();

    while (1) {
        tcp_sockaddr client_socket_info;
        int client_socket_info_len = sizeof(client_socket_info);
        tcp_socket client_socketfd = tcp_socket_accept(&(s->socket), &client_socket_info, &client_socket_info_len);
        
        new_connection_invoke(client_socketfd, connection_handler);

        printf("Client: %hhu.%hhu.%hhu.%hhu:%d\n", 
	    client_socket_info.sin_addr.s_addr & 0xFF000000UL,
	    client_socket_info.sin_addr.s_addr & 0x00FF0000UL,
	    client_socket_info.sin_addr.s_addr & 0x0000FF00UL,
	    client_socket_info.sin_addr.s_addr & 0x000000FFUL,
	    client_socket_info.sin_port
	);
    }

    s->status = closed;

    return 1;
}

void *connection_handler(unified_socket sock) {
    tcp_socket s = {
        .isClosed = false,
        .socket = sock
    };
    recieve_buffer buffer;
    init_buffer(&buffer, RECIEVE_BUFFER_LEN);
    
    ssize_t bytes_recieved;
    bool keep_alive;
    do {
        bytes_recieved = tcp_socket_recv(&s, buffer.buffer, buffer.size, 0);
        if (bytes_recieved > 0) {
            FILE *file = fopen("client_data.dat", "ab");
            fwrite(buffer.buffer, 1, bytes_recieved, file);
            fputc('\n', file);
            fclose(file);

            request req;
            init_request_struct(&req, 10000);
            parser_error pe = parse_http_request(buffer.buffer, bytes_recieved, &req);
            if (pe != PE_OK)
                printf("parser failure with code: %d\n", pe);
            keep_alive = request_handler(&req, &s);
            
            free_request_struct(&req);
        }
        else
            keep_alive = false;
    } while (keep_alive);
    tcp_socket_close(&s);
    
    free_buffer(&buffer);
    
    return NULL;
}

// Returns true if keep-alive is explicitly specified
bool request_handler(request *req, tcp_socket *s) {
    if (req->type == GET) {
        if (strcmp(req->file_path, "/")) {
            char final_path[PATH_LEN];
            snprintf(final_path, PATH_LEN * sizeof(char), "%s%s", ROOT_DIR, req->file_path);
            file_buffer file_buf;
            size_t file_size = get_file_lengthp(final_path);
            if (file_size != 0) {
                init_buffer(&file_buf, file_size);
                read_entire_file_to_buffer(final_path, &file_buf);
                request_buffer generated_req = create_request(&file_buf, guess_content_format_by_extension(final_path));
                
                ssize_t result = tcp_socket_send(s, generated_req.buffer, generated_req.size, 0);
                
                free_buffer(&file_buf);
                free_buffer(&generated_req);
            }
            else {
                printf("file_path failure: %s\n", req->file_path);
                send_static_html(s, "./assets/page_not_found.html");
            }
        }
        else
            send_welcome(s);
    }
    
    char *keep_alive_value = get_value_by_key(&req->headers, "Connection");
    if (keep_alive_value == NULL)
        return false;
    else if (!strcmp(keep_alive_value, "keep-alive"))
        return true;
    else
        return false;
}

void send_static_html(tcp_socket *s, const char *html_path) {
    struct __buffer webpage = get_static_page(html_path);
    request_buffer request = create_request(&webpage, CT_HTML);
    
    ssize_t result = tcp_socket_send(s, request.buffer, request.size, 0);
    // TODO: 
    // if (result < 0) ...
    free_buffer(&webpage);
    free_buffer(&request);
}

void send_welcome(tcp_socket *s) {
    struct __buffer webpage = generate_welcome_page();
    request_buffer request = create_request(&webpage, CT_HTML);

    ssize_t result = tcp_socket_send(s, request.buffer, request.size, 0);
    
    free_buffer(&request);
    free_buffer(&webpage);
}

content_types guess_content_format_by_extension(const char *file_name) {
    if (str_endswith(file_name, ".txt"))
        return CT_PLAIN;
    else if (str_endswith(file_name, ".html") ||
             str_endswith(file_name, ".htm"))
        return CT_HTML;
    else if (str_endswith(file_name, ".css"))
        return CT_CSS;
    else if (str_endswith(file_name, ".js"))
        return CT_JS;
    else if (str_endswith(file_name, ".json"))
        return CT_JSON;
    else if (str_endswith(file_name, ".jpg") ||
             str_endswith(file_name, ".jpe") ||
             str_endswith(file_name, ".jpeg"))
        return CT_JPEG;
    else if (str_endswith(file_name, ".xml"))
        return CT_XML;
    else if (str_endswith(file_name, ".png"))
        return CT_PNG;
    else if (str_endswith(file_name, ".gif"))
        return CT_GIF;
    else if (str_endswith(file_name, ".svg"))
        return CT_SVG;
    else if (str_endswith(file_name, ".mp4") ||
             str_endswith(file_name, ".m4a"))
        return CT_MP4;
    else if (str_endswith(file_name, ".wav") ||
             str_endswith(file_name, ".wave"))
        return CT_WAV;
    else if (str_endswith(file_name, ".mp3"))
        return CT_MPEG;
    else if (str_endswith(file_name, ".avi"))
        return CT_MSVIDEO;
    else if (str_endswith(file_name, ".pdf"))
        return CT_PDF;
    else if (str_endswith(file_name, ".zip"))
        return CT_ZIP;
    else if (str_endswith(file_name, ".tar"))
        return CT_TAR;
    else if (str_endswith(file_name, ".ico"))
        return CT_ICON;
    else
        return CT_PLAIN;
}
