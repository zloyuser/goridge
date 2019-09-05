#pragma once
#include "Connection.hpp"
#include "Frame.hpp"

namespace Goridge {
    class Relay {
        public:
            Relay(Connection * con);

            void send(Frame * frame);
            void send(const char * data, size_t size);
            Frame * receive();

            ~Relay();
        private:
            Connection * m_con;
    };
}
