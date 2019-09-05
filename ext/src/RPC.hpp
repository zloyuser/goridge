#pragma once
#include <cstring>
#include "Relay.hpp"

namespace Goridge {
    class RPC
    {
        public:
            RPC(Relay * relay);

            Frame * call(const char * method, const char * payload = "", size_t size = 0, bool raw = false);

            ~RPC();
        private:
            Relay * relay;
            union {
                uint64_t integer = 0;
                uint8_t byte[8];
            } m_seq;
    };
}
