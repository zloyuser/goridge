#pragma once
#include <cstring>

namespace Goridge {
    class Connection {
        public:
            virtual size_t send(const char * payload, size_t count) = 0;
            virtual size_t receive(char * payload, size_t count) = 0;

            virtual ~Connection();
    };
}
