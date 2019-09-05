#include "Frame.hpp"
#include <iostream>
#include <iomanip>

namespace Goridge {
    Frame::Frame(): m_body(""), m_size(size_t(0)), m_flags(0) {
    }

    Frame::Frame(uint8_t flags): m_body(""), m_size(size_t(0)), m_flags(flags) {
    }

    Frame::Frame(const char * body, size_t size, uint8_t flags): m_body(body), m_size(size), m_flags(flags) {
    }

    const char * Frame::pack() {
        return pack(m_body, m_size, m_flags);
    }

    const char * Frame::pack(const char * body, size_t size, uint8_t flags) {
        if (flags & PAYLOAD_NONE && size != 0) {
            throw FrameException("Unable to send payload with PAYLOAD_NONE flag");
        }

        char * bytes = new char[size + HEADER_SIZE];
        _size m_size(size);

        bytes[0] = flags;

        for (int i = 0; i < 8; i++) {
            bytes[1 + i] = m_size.byte[i];
            bytes[9 + i] = m_size.byte[7 - i];
        }

        if (!(flags & PAYLOAD_NONE)) {
            memcpy(bytes + HEADER_SIZE, body, size);
        }

        return bytes;
    }

    size_t Frame::length() {
        return m_size + HEADER_SIZE;
    }

    const uint8_t Frame::flags() {
        return m_flags;
    }

    const char * Frame::body() {
        return m_body;
    }

    size_t Frame::size() {
        return m_size;
    }

    bool Frame::isNone() {
        return m_flags & PAYLOAD_NONE;
    }

    bool Frame::isRaw() {
        return m_flags & PAYLOAD_RAW;
    }

    bool Frame::isError() {
        return m_flags & PAYLOAD_ERROR;
    }

    bool Frame::isControl() {
        return m_flags & PAYLOAD_CONTROL;
    }

    Frame::~Frame() {
    }
}
