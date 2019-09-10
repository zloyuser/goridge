#pragma once
#include "main/php.h"
#include "main/php_streams.h"
#include "Connection.hpp"

namespace Goridge {
class StreamConnection: public Connection {
    public:
        StreamConnection(php_stream * in, php_stream * out);

        static StreamConnection * connect(const char * dsn);

        size_t send(const char * payload, size_t count);
        size_t receive(char * payload, size_t count);

        ~StreamConnection();
    private:
        php_stream * m_in;
        php_stream * m_out;
};
}  // namespace Goridge
