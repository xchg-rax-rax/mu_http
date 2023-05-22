#include <iostream>

#include "HTTPHandler.h"
#include "ASIOGenericServer.h"

int main() {
    ASIOGenericServer<HTTPHandler> server;
    server.start_server(6969);
    return 0;
}

