#include "Frame.hpp"

namespace Goridge {
    Frame::Frame(uint8_t flags, size_t size): flags(flags), size(size) {
        body = (char *) malloc(size);
    }

    Frame::Frame(size_t size): size(size) {
        body = (char *) malloc(size);
    }

    const size_t Frame::pack(char * dst) {
        return pack(dst, body, size, flags);
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

    size_t Frame::length() const noexcept {
        return size + HEADER_SIZE;
    }

    bool Frame::isNone() const noexcept {
        return flags & PAYLOAD_NONE;
    }

    bool Frame::isRaw() const noexcept {
        return flags & PAYLOAD_RAW;
    }

    bool Frame::isError() const noexcept {
        return flags & PAYLOAD_ERROR;
    }

    bool Frame::isControl() const noexcept {
        return flags & PAYLOAD_CONTROL;
    }

    Frame::~Frame() {
        free(body);
    }
}  // namespace Goridge
