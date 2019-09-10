#pragma once
#include <exception>
#include <string>

using std::string;

namespace Goridge {
class GoridgeException: virtual public std::exception {
    public:
        explicit GoridgeException(const string& msg, const int code = 0): error_message(msg), error_code(code) {
        }

        virtual ~GoridgeException() throw() {
        }

        const char* what() const throw() {
            return error_message.c_str();
        }
    protected:
        const string error_message;
        const int error_code;
};

class FrameException: virtual public GoridgeException {
    public:
        explicit FrameException(const string& msg): GoridgeException(msg) {
        }

        virtual ~FrameException() throw() {
        }
};

class ConnectionException: virtual public GoridgeException {
    public:
        explicit ConnectionException(const string& msg): GoridgeException(msg) {
        }

        virtual ~ConnectionException() throw() {
        }
};

class RelayException: virtual public GoridgeException {
    public:
        explicit RelayException(const string& msg): GoridgeException(msg) {
        }

        virtual ~RelayException() throw() {
        }
};

class RPCException: virtual public GoridgeException {
    public:
        explicit RPCException(const string& msg): GoridgeException(msg) {
        }

        virtual ~RPCException() throw() {
        }
};

class ServiceException: virtual public GoridgeException {
    public:
        explicit ServiceException(const string& msg): GoridgeException(msg) {
        }

        virtual ~ServiceException() throw() {
        }
};
}  // namespace Goridge
