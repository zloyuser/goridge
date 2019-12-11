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
        uint8_t flags;
        size_t size;
        char * body;

        const size_t pack(char * dst);
        static const size_t pack(char * dst, const char * body, size_t size, uint8_t flags);

        size_t length() const noexcept;

        bool isNone() const noexcept;
        bool isRaw() const noexcept;
        bool isError() const noexcept;
        bool isControl() const noexcept;

        explicit Frame(size_t size);
        Frame(uint8_t flags, size_t size);
        ~Frame();
};
}  // namespace Goridge
