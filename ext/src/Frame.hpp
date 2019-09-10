#pragma once
#include <cstring>
#include "Exceptions.hpp"

namespace Goridge {
static const unsigned int BUFFER_SIZE = 65536;
static const unsigned int HEADER_SIZE = 17;

static const unsigned char PAYLOAD_NONE    = 2;
static const unsigned char PAYLOAD_RAW     = 4;
static const unsigned char PAYLOAD_ERROR   = 8;
static const unsigned char PAYLOAD_CONTROL = 16;

union _size {
    _size() = default;
    _size(size_t x): integer(x) {}
    _size(uint8_t x[8]): byte{x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]} {}
    ~_size() {}
    size_t integer;
    uint8_t byte[8];
};

class Frame {
    public:
        explicit Frame(uint8_t flags);
        Frame(const char * body, size_t size, uint8_t flags = 0);

        const char * pack();
        static const char * pack(const char * body, size_t size, uint8_t flags);

        const char * body() const noexcept;
        size_t size() const noexcept;
        size_t length() const noexcept;

        uint8_t flags() const noexcept;
        bool isNone() const noexcept;
        bool isRaw() const noexcept;
        bool isError() const noexcept;
        bool isControl() const noexcept;

        ~Frame();
    private:
        const char * m_body;
        size_t m_size;
        uint8_t m_flags;
};
}  // namespace Goridge
