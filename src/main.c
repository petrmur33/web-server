#include "main.h"

int main(void) {
    server web_server;
    
    if (!create_server(&web_server)) {
        error("create_server\n");
        return -1;
    }

    if (!run_server(&web_server)) {
        error("run_server\n");
        return -1;
    }

    return 0;
}
