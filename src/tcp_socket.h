#pragma once

#include "os_definition.h"

#include <stdbool.h>

#if defined(WINDOWS)
    #include <winsock2.h>
#elif defined(POSIX)
    #include <errno.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <sys/fcntl.h>
    #include <unistd.h>
#endif

#include "utils.h"
#include "logger.h"

#if defined(WINDOWS)
    typedef SOCKET unified_socket;
#elif defined(POSIX)
    typedef int unified_socket;
#endif

typedef struct tcp_socket {
    unified_socket socket;
    bool isClosed;
} tcp_socket;
typedef struct sockaddr_in tcp_sockaddr;

void cleanup_tcp_sockets(void);
void tcp_socket_close(tcp_socket *s);
void initialize_tcp_sockets(void);
tcp_socket *tcp_socket_init(tcp_socket *s, const char *ip_address, short port);
tcp_socket *tcp_socket_listen(tcp_socket *s, int backlog);
tcp_socket tcp_socket_accept(tcp_socket *s, tcp_sockaddr *addr, int *addr_len);
ssize_t tcp_socket_recv(tcp_socket *s, void *buf, int len, int flag);
ssize_t tcp_socket_send(tcp_socket *s, void *buf, int len, int flags);
