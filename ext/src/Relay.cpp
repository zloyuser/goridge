#include "Exceptions.hpp"
#include "Relay.hpp"

namespace Goridge {
    Relay::Relay(Connection * con): m_con(con) {
    }

    void Relay::send(Frame * frame) {
        char data[frame->length()];

        frame->pack(data);

        send(data, frame->length());
    }

    void Relay::send(const char * data, size_t size) {
        m_con->send(data, size);
    }

    void Relay::receive(Frame * dst) {
        char header[HEADER_SIZE];

        m_con->receive(header, HEADER_SIZE);

        for (int i = 0; i < 8; i++) {
            if (header[i + 1] != header[HEADER_SIZE - 1 - i]) {
                throw FrameException("Frame checksum mismatch");
            }
        }

        _size size_le;

        memcpy(&size_le, header + 1, 8);

        dst->flags = header[0];
        dst->size = size_le.integer;

        if (dst->size == 0) {
            return;
        }

        size_t to_read = dst->size, offset = 0, chunk, _read;

        dst->body = (char *) malloc(dst->size + 1);

        while (to_read > 0) {
            chunk = to_read > BUFFER_SIZE ? BUFFER_SIZE : to_read;
            _read = m_con->receive(dst->body + offset, chunk);

            to_read -= _read;
            offset  += _read;
        }

        dst->body[dst->size] = '\0';
    }

    Relay::~Relay() {
    }
}  // namespace Goridge
