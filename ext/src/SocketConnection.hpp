#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Connection.hpp"

namespace Goridge {
class SocketConnection: public Connection {
    public:
        explicit SocketConnection(int sock);

        static SocketConnection * connect(const char * address);
        static SocketConnection * connect(const char * address, uint16_t port);

        size_t send(const char * payload, size_t length);
        size_t receive(char * payload, size_t length);

        ~SocketConnection();
    private:
        int m_sock;
};
}  // namespace Goridge
