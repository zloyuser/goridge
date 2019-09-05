#include "Exceptions.hpp"
#include "StreamConnection.hpp"

namespace Goridge {
    StreamConnection::StreamConnection(php_stream * in, php_stream * out): m_in(in), m_out(out) {
    };

    StreamConnection * StreamConnection::connect(const char * dsn) {
        struct timeval tv {0, 0};

        int * code;
        zend_string * errstr = NULL;

        php_stream_context *context = php_stream_context_alloc();
        php_stream * stream = php_stream_xport_create(dsn, strlen(dsn), REPORT_ERRORS, STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT, NULL, &tv, context, &errstr, code);

        if (stream == NULL) {
            throw ConnectionException(ZSTR_VAL(errstr));
        }

        return new StreamConnection(stream, stream);
    };

    size_t StreamConnection::send(const char * payload, size_t count) {
        return php_stream_write(m_out, payload, count);
    }

    size_t StreamConnection::receive(char * payload, size_t count) {
        return php_stream_read(m_in, payload, count);
    }

    StreamConnection::~StreamConnection() {
        php_stream_close(m_in);
        php_stream_close(m_out);
    }
}
