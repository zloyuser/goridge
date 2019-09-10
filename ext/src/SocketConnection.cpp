#include "Exceptions.hpp"
#include "SocketConnection.hpp"

using std::copy;

namespace Goridge {
    SocketConnection::SocketConnection(int sock): m_sock(sock) {
    }

    SocketConnection * SocketConnection::connect(const char * address) {
        sockaddr_un s_addr;
        int sock;

        if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
            throw ConnectionException("Cant open UNIX socket");
        }

        s_addr.sun_family = AF_UNIX;
        strcpy(s_addr.sun_path, address);

        if (::connect(sock, (sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
            throw ConnectionException("Cant connect to UNIX socket");
        }

        return new SocketConnection(sock);
    }

    SocketConnection * SocketConnection::connect(const char * address, uint16_t port) {
        sockaddr_in s_addr;
        int sock;

        if (inet_pton(AF_INET, address, &s_addr.sin_addr) <= 0) {
            throw ConnectionException("Invalid TCP address");
        }

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw ConnectionException("Cant open TCP socket");
        }

        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(port);

        if (::connect(sock, (sockaddr *) &s_addr, sizeof(s_addr)) < 0) {
            throw ConnectionException("Cant connect to TCP socket");
        }

        return new SocketConnection(sock);
    }

    size_t SocketConnection::send(const char * payload, size_t length) {
        return ::send(m_sock, payload, length, 0);
    }

    size_t SocketConnection::receive(char * payload, size_t length) {
        return ::recv(m_sock, payload, length, MSG_WAITALL);
    }

    SocketConnection::~SocketConnection() {
        ::close(m_sock);
    }
}  // namespace Goridge
