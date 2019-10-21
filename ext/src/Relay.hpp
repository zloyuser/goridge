#pragma once
#include "Connection.hpp"
#include "Frame.hpp"

namespace Goridge {
class Relay {
    public:
        explicit Relay(Connection * con);

        void send(Frame * frame);
        void send(const char * data, size_t size);
        void receive(Frame * dst);

        ~Relay();
    private:
        Connection * m_con;
};
}  // namespace Goridge
