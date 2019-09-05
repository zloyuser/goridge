#include "RPC.hpp"
#include "Exceptions.hpp"

using std::string;

namespace Goridge {
    RPC::RPC(Relay * relay): relay(relay) {
    }

    Frame * RPC::call(const char * method, const char * payload, size_t size, bool raw) {
        string data(string(method).append((char *) m_seq.byte, 8));

        Frame * f_control = new Frame(data.c_str(), data.size(), PAYLOAD_CONTROL | PAYLOAD_RAW);
        Frame * f_payload = new Frame(payload, size, raw ? PAYLOAD_RAW : 0);
        size_t length = f_control->length() + f_payload->length();

        char * binary = new char[length];

        memcpy(binary, f_control->pack(), f_control->length());
        memcpy(binary + f_control->length(), f_payload->pack(), f_payload->length());

        relay->send(binary, length);

        delete f_control, f_payload, binary;

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
}
