#include "Frame.hpp"

namespace Goridge {
    Frame::Frame(uint8_t flags): m_body(""), m_size(size_t(0)), m_flags(flags) {
    }

    Frame::Frame(const char * body, size_t size, uint8_t flags):
        m_body(body), m_size(size), m_flags(flags) {
    }

    const char * Frame::pack() {
        char * bytes = new char[m_size + HEADER_SIZE];

        pack(bytes, m_body, m_size, m_flags);

        return bytes;
    }

    const size_t Frame::pack(char * dst, const char * body, size_t size, uint8_t flags) {
        if (flags & PAYLOAD_NONE && size != 0) {
            throw FrameException("Unable to send payload with PAYLOAD_NONE flag");
        }

        _size m_size(size);

        dst[0] = flags;

        for (int i = 0; i < 8; i++) {
            dst[1 + i] = m_size.byte[i];
            dst[9 + i] = m_size.byte[7 - i];
        }

        if (!(flags & PAYLOAD_NONE)) {
            memcpy(dst + HEADER_SIZE, body, size);
        }

        return size + HEADER_SIZE;
    }

    const char * Frame::body() const noexcept {
        return m_body;
    }

    size_t Frame::size() const noexcept {
        return m_size;
    }

    size_t Frame::length() const noexcept {
        return m_size + HEADER_SIZE;
    }

    uint8_t Frame::flags() const noexcept {
        return m_flags;
    }

    bool Frame::isNone() const noexcept {
        return m_flags & PAYLOAD_NONE;
    }

    bool Frame::isRaw() const noexcept {
        return m_flags & PAYLOAD_RAW;
    }

    bool Frame::isError() const noexcept {
        return m_flags & PAYLOAD_ERROR;
    }

    bool Frame::isControl() const noexcept {
        return m_flags & PAYLOAD_CONTROL;
    }

    Frame::~Frame() {
    }
}  // namespace Goridge
