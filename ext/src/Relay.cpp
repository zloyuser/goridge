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

    void Relay::send(const char * payload, size_t size) {
        m_con->send(payload, size);
    }

    Frame * Relay::receive() {
        char header[HEADER_SIZE];
        _size size_le;

        m_con->receive(header, HEADER_SIZE);

        for (int i = 0; i < 8; i++) {
            if (header[i + 1] != header[HEADER_SIZE - 1 - i]) {
                throw FrameException("Frame checksum mismatch");
            }

            size_le.byte[i] = header[i + 1];
        }

        Frame * dst = new Frame(header[0], size_le.integer);

        if (dst->size != 0) {
            size_t to_read = dst->size, offset = 0, chunk, _read;

            while (to_read > 0) {
                chunk = to_read > BUFFER_SIZE ? BUFFER_SIZE : to_read;
                _read = m_con->receive(dst->body + offset, chunk);

                to_read -= _read;
                offset  += _read;
            }

            dst->body[dst->size] = '\0';
        }

        return dst;
    }

    Relay::~Relay() {
    }
}  // namespace Goridge
