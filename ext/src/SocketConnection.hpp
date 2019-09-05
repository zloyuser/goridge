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
            SocketConnection(int sock);

            static SocketConnection * connect(const char * address);
            static SocketConnection * connect(const char * address, uint16_t port);

            size_t send(const char * payload, size_t length);
            size_t receive(char * payload, size_t length);

            ~SocketConnection();
        private:
            const char * m_address;
            uint16_t m_port;

            bool m_tcp;
            int m_sock;
            bool m_connected = false;
    };
}
