#include "RPC.hpp"
#include "Exceptions.hpp"

using std::string;

namespace Goridge {
    RPC::RPC(Relay * relay): relay(relay) {
    }

    Frame * RPC::call(const char * method, const char * payload, size_t size, bool raw) {
        string data(string(method).append(m_seq.chars, 8));
        string binary;

        Frame * f_control = new Frame(data.data(), data.size(), PAYLOAD_CONTROL | PAYLOAD_RAW);
        Frame * f_payload = new Frame(payload, size, raw ? PAYLOAD_RAW : 0);

        binary = binary
            .append(f_control->pack(), f_control->length())
            .append(f_payload->pack(), f_payload->length());

        relay->send(binary.data(), binary.size());

        delete f_control, f_payload;

        Frame * control = relay->receive();

        if (!control->isControl()) {
            throw RPCException("RPC response header is missing");
        }

        uint64_t _seq;
        char _method[control->size() - 7];

        memcpy(&_seq, control->body() + control->size() - 8, 8);
        memcpy(&_method, control->body(), control->size() - 8);
        _method[control->size() - 8] = '\0';

        if (strcmp(method, _method) != 0) {
            throw RPCException("RPC method mismatch");
        }

        if (m_seq.integer != _seq) {
            throw RPCException("RPC seq mismatch");
        }

        m_seq.integer++;

        return relay->receive();
    }

    RPC::~RPC() {
    }
}  // namespace Goridge
