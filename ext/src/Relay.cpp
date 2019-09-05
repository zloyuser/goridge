#include "Exceptions.hpp"
#include "Relay.hpp"

namespace Goridge {
    Relay::Relay(Connection * con): m_con(con) {
    }

    void Relay::send(Frame * frame) {
        send(frame->pack(), frame->length());
    }

    void Relay::send(const char * data, size_t size) {
        m_con->send(data, size);
    }

    Frame * Relay::receive() {
        char * header = new char[HEADER_SIZE];

        m_con->receive(header, HEADER_SIZE);

        _size * size_le = new _size;
        _size * size_be = new _size;

        size_t usize = sizeof(uint64_t);

        memcpy(size_le, header + 1, usize);
        memcpy(size_be, header + 1 + usize, usize);

        for (int i = 0; i < usize; i++) {
            if (size_le->byte[i] != size_be->byte[usize - 1 - i]) {
                throw new FrameException("Frame checksum mismatch");
            }
        }

        uint8_t flags = header[0];
        uint64_t size = size_le->integer;

        delete header, size_be, size_le;

        if (size == 0) {
            return new Frame(flags);
        }

        uint64_t to_read = size, offset = 0;

        char * body = new char[size + 1];

        while (to_read > 0) {
            size_t chunk_size = to_read > BUFFER_SIZE ? BUFFER_SIZE : to_read;
            char * chunk = new char[chunk_size];

            size_t _read = m_con->receive(chunk, chunk_size);

            memcpy(body + offset, chunk, _read);

            to_read -= _read;
            offset += _read;
        }

        body[size] = '\0';

        return new Frame(body, size, flags);
    }

    Relay::~Relay() {
    }
}
