#pragma once
#include <string>
#include "Relay.hpp"

namespace Goridge {
class RPC {
    public:
        explicit RPC(Relay * relay);

        Frame * call(const char * method, const char * payload = "", size_t size = 0, bool raw = false);

        ~RPC();
    private:
        Relay * relay;
        union {
            uint64_t integer = 0;
            char chars[8];
        } m_seq;
};
}  // namespace Goridge
