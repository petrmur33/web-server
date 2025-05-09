#include "tcp_socket.h"

static int socket_error_handler(char *msg_format);

char error_message[ERROR_MESSAGE_LEN];

// NOTE: msg_format must have %d for error code
// returns -1 if connection was reset or aborted
static int socket_error_handler(char *msg_format) {
    int error_code;
    #if defined(WINDOWS)
    error_code = WSAGetLastError();
    if (error_code == WSAECONNRESET ||
        error_code == WSAECONNABORTED ||
        error_code == WSAETIMEDOUT)
        return -1;
    #elif defined(POSIX)
    error_code = errno;
    if (error_code == ECONNRESET)
	return -1;
    #endif
    snprintf(error_message, ERROR_MESSAGE_LEN, msg_format, error_code);
    logger_write_global(error_message, LOGGER_ERROR);
    cleanup_tcp_sockets();
    error(error_message);

    return 0;
}

void cleanup_tcp_sockets(void) {
    #ifdef WINDOWS
    WSACleanup();
    #endif
}

void tcp_socket_close(tcp_socket *s) {
    char log[50];
    snprintf(log, 50, "socket_close: %d", s->socket);
    logger_write_global(log, LOGGER_DEBUG);

    #if defined(WINDOWS)
    if (closesocket(s->socket))
    #elif defined(POSIX)
    if (close(s->socket))
    #endif
        socket_error_handler("close function failure with code: %d\n");
    
    s->isClosed = true;
}

void initialize_tcp_sockets(void) {
    #if defined(WINDOWS)
    WSADATA wsaData = {};
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result)
        socket_error_handler("WSAStartup failure with code: %d\n");
    #elif defined(UNKNOWN_OS)
    socket_error_handler("Unknown os failure %d\n");
    #endif
    ;
}

tcp_socket *tcp_socket_init(tcp_socket *s, const char *ip_address, short port) {
    s->isClosed = false;
    s->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    #if defined(WINDOWS)
    if (s->socket == INVALID_SOCKET)
    #elif defined(POSIX)
    if (s->socket == -1)
    #endif
        socket_error_handler("socket function failure with code: %d\n");
    
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip_address)
    };

    int opt; // this is option
    if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        socket_error_handler("setsockopt function failure with code: %d\n");

    // TODO: make timeout option for linux
    #if defined(WINDOWS)
    DWORD timeout = 3000;
    if (setsockopt(s->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
        socket_error_handler("setsockopt function failure with code: %d\n");
    #endif

    if (bind(s->socket, (struct sockaddr *) &address, sizeof(address)))
        socket_error_handler("bind function failure with code: %d\n");
    
    return s;
}

tcp_socket *tcp_socket_listen(tcp_socket *s, int backlog) {
    char log[50];
    snprintf(log, 50, "socket_listen: %d", s->socket);
    logger_write_global(log, LOGGER_DEBUG);
    
    if (listen(s->socket, backlog))
        socket_error_handler("listen function failure with code: %d\n");
    
    return s;
}

// returns number of bytes recieved
// return -1 if remote socket should close
ssize_t tcp_socket_recv(tcp_socket *s, void *buf, int len, int flag) {
    char log[50];
    snprintf(log, 50, "socket_recv: %d", s->socket);
    logger_write_global(log, LOGGER_DEBUG);
    
    #if defined(WINDOWS)
    ssize_t result = recv(s->socket, buf, len, flag);
    if (result == SOCKET_ERROR) {
    #elif defined(POSIX)
    ssize_t result = recv(s->socket, buf, len, flag);
    if (result == -1) {
    #endif
        int error = socket_error_handler("recv function failure with code: %d\n");
        if (error == -1) return -1;
    }
    
    return result;
}

// returns new socket
tcp_socket tcp_socket_accept(tcp_socket *s, tcp_sockaddr *addr, int *addr_len) {
    char log[50];
    snprintf(log, 50, "socket_accept: %d", s->socket);
    logger_write_global(log, LOGGER_DEBUG);
    
    #if defined(WINDOWS)
    unified_socket new_socket = accept(s->socket, (struct sockaddr *) addr, addr_len);
    if (new_socket == INVALID_SOCKET)
    #elif defined(POSIX)
    unified_socket new_socket = accept(s->socket, (struct sockaddr *) addr, (socklen_t *) addr_len);
    if (new_socket == -1)
    #endif
        socket_error_handler("accept function failure with code: %d\n");
    
    tcp_socket new_tcp_socket = {
        .socket = new_socket,
        .isClosed = false
    };
    
    snprintf(log, 50, "socket_accept returned: %d -> %d", s->socket, new_tcp_socket.socket);
    logger_write_global(log, LOGGER_DEBUG);
    
    return new_tcp_socket;
}

// return -1 if remote socket should close
ssize_t tcp_socket_send(tcp_socket *s, void *buf, int len, int flags) {
    char log[50];
    snprintf(log, 50, "socket_send: %d", s->socket);
    logger_write_global(log, LOGGER_DEBUG);
    
    ssize_t result = send(s->socket, buf, len, flags);
    #if defined(WINDOWS)
    if (result == SOCKET_ERROR) {
    #elif defined(POSIX)
    if (result == -1) {
    #endif
        int error = socket_error_handler("send function failure with code: %d\n");
        if (error == -1) return -1;
    }
        
    return result;
}
