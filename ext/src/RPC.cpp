#include "RPC.hpp"
#include "Exceptions.hpp"

namespace Goridge {
    RPC::RPC(Relay * relay): relay(relay) {
    }

    Frame * RPC::call(const char * method, const char * payload, size_t size, bool raw) {
        size_t method_size = strlen(method);
        size_t request_size = method_size + 8;

        size_t l_method = HEADER_SIZE + request_size;
        size_t l_payload = HEADER_SIZE + size;

        char request[request_size];
        char binary[l_method + l_payload];

        memcpy(request, method, method_size);
        memcpy(request + strlen(method), m_seq.chars, 8);

        Frame::pack(binary, request, request_size, PAYLOAD_CONTROL | PAYLOAD_RAW);
        Frame::pack(binary + l_method, payload, size, raw ? PAYLOAD_RAW : 0);

        relay->send(binary, l_method + l_payload);
        Frame * control = relay->receive();

        if (!control->isControl()) {
            throw RPCException("RPC response header is missing");
        }

        uint64_t _seq;
        char _method[control->size - 7];

        memcpy(&_seq, control->body + control->size - 8, 8);
        memcpy(&_method, control->body, control->size - 8);

        _method[control->size - 8] = '\0';

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
