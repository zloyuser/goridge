/* goridge extension for PHP */

#include "php_goridge.h"
#include "src/Frame.hpp"
#include "src/Relay.hpp"
#include "src/RPC.hpp"
#include "src/StreamConnection.hpp"

using Goridge::Frame;
using Goridge::Relay;
using Goridge::RPC;
using Goridge::StreamConnection;
using Goridge::ConnectionException;
using Goridge::FrameException;
using Goridge::RPCException;
using Goridge::ServiceException;

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

// === EXCEPTIONS ===
static zend_class_entry * goridge_exception_ce = NULL;
static zend_class_entry * frame_exception_ce = NULL;
static zend_class_entry * stream_exception_ce = NULL;
static zend_class_entry * json_exception_ce = NULL;
static zend_class_entry * rpc_exception_ce = NULL;
static zend_class_entry * service_exception_ce = NULL;

// === FRAME ===
typedef struct _frame_object  {
    Frame * obj;
    zend_object std;
} frame_object;

static zend_class_entry * frame_ce = NULL;
static zend_object_handlers frame_handlers;

static zend_object * frame_create(zend_class_entry * ce) {
    frame_object * frame = (frame_object *) ecalloc(1, sizeof(frame_object) + zend_object_properties_size(ce));

    zend_object * ret = ((zend_object *)(frame + 1)) - 1;

    zend_object_std_init(ret, ce);
    object_properties_init(ret, ce);

    ret->handlers = &frame_handlers;

    return ret;
}

static frame_object * frame_from_zend(zend_object * objval)
{
    return ((frame_object *)(objval + 1)) - 1;
}

static void frame_free(zend_object * zobj) {
    frame_object * frame = frame_from_zend(zobj);

    delete frame->obj;

    zend_object_std_dtor(zobj);
}

static zval * frame_read_property(zval * object, zval * name, int type, void ** cache_slot, zval * rv)
{
    frame_object * frame = frame_from_zend(Z_OBJ_P(object));

    if (strcmp(Z_STRVAL_P(name), "body") == 0) {
        zend_string * body = zend_string_init(frame->obj->body(), frame->obj->size(), 0);

        ZVAL_NEW_STR(rv, body);

        return rv;
    }

    if (strcmp(Z_STRVAL_P(name), "flags") == 0) {
        ZVAL_LONG(rv, frame->obj->flags());

        return rv;
    }

    return rv;
}

static void frame_write_property(zval * object, zval * member, zval * value, void ** cache_slot)
{
    // NOT IMPLEMENTED
}

static int frame_has_property(zval * object, zval * name, int type, void ** cache_slot)
{
    zval rv;
    int retval = 0;

    zval * prop = frame_read_property(object, name, BP_VAR_IS, cache_slot, &rv);

    if (prop != &EG(uninitialized_zval)) {
        if (type == 2) {
            retval = 1;
        } else if (type == 1) {
            retval = zend_is_true(prop);
        } else if (type == 0) {
            retval = (Z_TYPE_P(prop) != IS_NULL);
        }
    }

    return retval;
}

static HashTable * frame_get_properties(zval * object)
{
    zval zv;

    frame_object * frame = frame_from_zend(Z_OBJ_P(object));

    HashTable * props = zend_std_get_properties(object);
    zend_string * body = zend_string_init(frame->obj->body(), frame->obj->size(), 0);

    ZVAL_NEW_STR(&zv, body);
    zend_hash_str_update(props, "body", sizeof("body") - 1, &zv);
    ZVAL_LONG(&zv, frame->obj->flags());
    zend_hash_str_update(props, "flags", sizeof("flags") - 1, &zv);

    return props;
}

PHP_METHOD(Frame, __construct)
{
    char * body; size_t body_len;
    zend_long flags = 0;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|l", &body, &body_len, &flags) == FAILURE) {
        return;
    }

    frame_object * frame = frame_from_zend(Z_OBJ_P(getThis()));

    frame->obj = new Frame(body, body_len, flags);
}

PHP_METHOD(Frame, isRaw)
{
    frame_object * frame = frame_from_zend(Z_OBJ_P(getThis()));

    RETURN_BOOL(frame->obj->isRaw());
}

PHP_METHOD(Frame, isError)
{
    frame_object * frame = frame_from_zend(Z_OBJ_P(getThis()));

    RETURN_BOOL(frame->obj->isError());
}

PHP_METHOD(Frame, isControl)
{
    frame_object * frame = frame_from_zend(Z_OBJ_P(getThis()));

    RETURN_BOOL(frame->obj->isControl());
}

PHP_METHOD(Frame, __toString)
{
    frame_object * frame = frame_from_zend(Z_OBJ_P(getThis()));

    const char * bytes = frame->obj->pack();

    RETURN_STRINGL(bytes, frame->obj->length());
}

static zend_function_entry frame_methods[] = {
    PHP_ME(Frame, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Frame, isRaw, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Frame, isError, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Frame, isControl, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Frame, __toString, NULL, ZEND_ACC_PUBLIC)

    PHP_FE_END
};
// === FRAME ===

// === RELAY ===
typedef struct _relay_object  {
    Relay * obj;
    zend_object std;
} relay_object;

static zend_class_entry * relay_ce = NULL;
static zend_object_handlers relay_handlers;

static zend_object * relay_create(zend_class_entry * ce) {
    relay_object * relay = (relay_object *) ecalloc(1, sizeof(relay_object) + zend_object_properties_size(ce));

    zend_object * ret = ((zend_object *)(relay + 1)) - 1;

    zend_object_std_init(ret, ce);
    object_properties_init(ret, ce);

    ret->handlers = &relay_handlers;

    return ret;
}

static relay_object * relay_from_zend(zend_object * objval)
{
    return ((relay_object *)(objval + 1)) - 1;
}

static void relay_free(zend_object * zobj) {
    relay_object * relay = relay_from_zend(zobj);

    delete relay->obj;

    zend_object_std_dtor(zobj);
}

PHP_METHOD(Relay, __construct)
{
    zval * input, * output;

    php_stream * i_stream, * o_stream;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz", &input, &output) == FAILURE) {
        return;
    }

    php_stream_from_zval(i_stream, input);
    php_stream_from_zval(o_stream, output);

    relay_object * relay = relay_from_zend(Z_OBJ_P(getThis()));

    relay->obj = new Relay(new StreamConnection(i_stream, o_stream));
}

PHP_METHOD(Relay, pipes)
{
    if (zend_parse_parameters_none_throw() == FAILURE) {
        return;
    }

    php_stream * s_in, * s_out;
    php_stream_context * sc_in = NULL, * sc_out = NULL;

    s_in  = php_stream_open_wrapper_ex("php://stdin",  "rb", 0, NULL, sc_in);
    s_out = php_stream_open_wrapper_ex("php://stdout", "wb", 0, NULL, sc_out);

    relay_object *relay = NULL;

    object_init_ex(return_value, relay_ce);

    relay = relay_from_zend(Z_OBJ_P(return_value));
    relay->obj = new Relay(new StreamConnection(s_in, s_out));
}

PHP_METHOD(Relay, connect)
{
    char * host; size_t host_len;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &host, &host_len) == FAILURE) {
        return;
    }

    try {
        StreamConnection * con = StreamConnection::connect(host);

        relay_object * relay = NULL;

        object_init_ex(return_value, relay_ce);

        relay = relay_from_zend(Z_OBJ_P(return_value));
        relay->obj = new Relay(con);
    } catch (ConnectionException e) {
        zend_throw_exception(stream_exception_ce, e.what(), 0);
    }
}

PHP_METHOD(Relay, send)
{
    char * payload; size_t payload_len;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &payload, &payload_len) == FAILURE) {
        return;
    }

    relay_object * relay = relay_from_zend(Z_OBJ_P(getThis()));

    relay->obj->send(payload, payload_len);

    RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(Relay, receive)
{
    relay_object * relay = relay_from_zend(Z_OBJ_P(getThis()));

    try {
        Frame * response = relay->obj->receive();

        frame_object * frame = NULL;

        object_init_ex(return_value, frame_ce);

        frame = frame_from_zend(Z_OBJ_P(return_value));
        frame->obj = response;
    } catch (FrameException e) {
        zend_throw_exception(frame_exception_ce, e.what(), 0);
    }
}

static zend_function_entry relay_methods[] = {
    PHP_ME(Relay, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Relay, pipes, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Relay, connect, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Relay, send, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Relay, receive, NULL, ZEND_ACC_PUBLIC)

    PHP_FE_END
};
// === RELAY ===

// === RPC ===
typedef struct _rpc_object  {
    RPC * obj;
    zend_object std;
} rpc_object;

static zend_class_entry * rpc_ce = NULL;
static zend_object_handlers rpc_handlers;

static zend_object * rpc_create(zend_class_entry * ce) {
    rpc_object * objval = (rpc_object *) ecalloc(1, sizeof(rpc_object) + zend_object_properties_size(ce));

    zend_object * ret = ((zend_object *)(objval + 1)) - 1;

    zend_object_std_init(ret, ce);
    object_properties_init(ret, ce);

    ret->handlers = &rpc_handlers;

    return ret;
}

static rpc_object * rpc_from_zend(zend_object * objval)
{
    return ((rpc_object *)(objval + 1)) - 1;
}

static void rpc_free(zend_object * zobj) {
    rpc_object * rpc = rpc_from_zend(zobj);

    delete rpc->obj;

    zend_object_std_dtor(zobj);
}

PHP_METHOD(RPC, __construct)
{
    zval * val;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "o", &val) == FAILURE) {
        return;
    }

    rpc_object * rpc = rpc_from_zend(Z_OBJ_P(getThis()));

    if (instanceof_function(Z_OBJCE_P(val), relay_ce) != 0) {
        relay_object * relay = relay_from_zend(Z_OBJ_P(val));

        rpc->obj = new RPC(relay->obj);
    } else {
        zend_type_error("Invalid arguments for RPC::__construct().");
    }
}

PHP_METHOD(RPC, connect)
{
    char * host; size_t host_len;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &host, &host_len) == FAILURE) {
        return;
    }

    try {
        StreamConnection * con = StreamConnection::connect(host);

        rpc_object *rpc = NULL;

        object_init_ex(return_value, rpc_ce);

        rpc = rpc_from_zend(Z_OBJ_P(return_value));

        rpc->obj = new RPC(new Relay(con));
    } catch (ConnectionException e) {
        zend_throw_exception(stream_exception_ce, e.what(), 0);
    }
}

PHP_METHOD(RPC, call)
{
    zval * method, * payload;

    bool raw = false;

    if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "zz|b", &method, &payload, &raw) == FAILURE) {
        return;
    }
    
    zend_string * data;

    if (raw) {
        data = zend_string_init(Z_STRVAL_P(payload), Z_STRLEN_P(payload), 0);
    } else {
        smart_str buf = {0};

        if (php_json_encode(&buf, payload, 0) == FAILURE) {
            smart_str_free(&buf);

            zend_throw_exception(json_exception_ce, "JSON encode error", JSON_G(error_code));

            return;
        }

        data = smart_str_extract(&buf);
    }

    try {
        rpc_object * rpc = rpc_from_zend(Z_OBJ_P(getThis()));

        Frame * response = rpc->obj->call(Z_STRVAL_P(method), ZSTR_VAL(data), ZSTR_LEN(data), raw);

        zend_string_free(data);

        if (response->isError()) {
            zend_throw_exception(service_exception_ce, response->body(), 0);

            return;
        }

        if (response->isRaw()) {
            RETURN_STRINGL(response->body(), response->size());

            return;
        }

        if (php_json_decode(return_value, (char *) response->body(), response->size(), true, PHP_JSON_PARSER_DEFAULT_DEPTH) == FAILURE) {
            zend_throw_exception(json_exception_ce, "JSON decode error.", JSON_G(error_code));

            return;
        }
    } catch (RPCException e) {
        zend_throw_exception(rpc_exception_ce, e.what(), 0);
    }
}

static zend_function_entry rpc_methods[] = {
    PHP_ME(RPC, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(RPC, connect, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(RPC, call, NULL, ZEND_ACC_PUBLIC)

    PHP_FE_END
};
// === RPC ===

PHP_MINIT_FUNCTION(goridge)
{
#if defined(ZTS) && defined(COMPILE_DL_GORIDGE)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    // EXCEPTIONS
    zend_class_entry _goridge_exception_ce;
    INIT_CLASS_ENTRY(_goridge_exception_ce, "Spiral\\Goridge\\Exception\\GoridgeException", NULL);
    goridge_exception_ce = zend_register_internal_class_ex(&_goridge_exception_ce, zend_ce_exception);

    zend_class_entry _frame_exception_ce;
    INIT_CLASS_ENTRY(_frame_exception_ce, "Spiral\\Goridge\\Exception\\FrameException", NULL);
    frame_exception_ce = zend_register_internal_class_ex(&_frame_exception_ce, goridge_exception_ce);

    zend_class_entry _stream_exception_ce;
    INIT_CLASS_ENTRY(_stream_exception_ce, "Spiral\\Goridge\\Exception\\StreamException", NULL);
    stream_exception_ce = zend_register_internal_class_ex(&_stream_exception_ce, goridge_exception_ce);

    zend_class_entry _json_exception_ce;
    INIT_CLASS_ENTRY(_json_exception_ce, "Spiral\\Goridge\\Exception\\JSONException", NULL);
    json_exception_ce = zend_register_internal_class_ex(&_json_exception_ce, goridge_exception_ce);

    zend_class_entry _rpc_exception_ce;
    INIT_CLASS_ENTRY(_rpc_exception_ce, "Spiral\\Goridge\\Exception\\RPCException", NULL);
    rpc_exception_ce = zend_register_internal_class_ex(&_rpc_exception_ce, goridge_exception_ce);

    zend_class_entry _service_exception_ce;
    INIT_CLASS_ENTRY(_service_exception_ce, "Spiral\\Goridge\\Exception\\ServiceException", NULL);
    service_exception_ce = zend_register_internal_class_ex(&_service_exception_ce, rpc_exception_ce);

    // FRAME
    zend_class_entry _frame_ce;
    INIT_CLASS_ENTRY(_frame_ce, "Spiral\\Goridge\\Frame", frame_methods);

    frame_ce = zend_register_internal_class(&_frame_ce);
    frame_ce->create_object = frame_create;

    memcpy(&frame_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    frame_handlers.offset = XtOffsetOf(frame_object, std);
    frame_handlers.read_property = frame_read_property;
    frame_handlers.write_property = frame_write_property;
    frame_handlers.has_property = frame_has_property;
    frame_handlers.get_properties = frame_get_properties;
    frame_handlers.free_obj = frame_free;
    frame_handlers.dtor_obj = zend_objects_destroy_object;

    zend_declare_class_constant_long(frame_ce, "PAYLOAD_NONE",  sizeof("PAYLOAD_NONE") - 1, Goridge::PAYLOAD_NONE TSRMLS_CC);
    zend_declare_class_constant_long(frame_ce, "PAYLOAD_RAW",  sizeof("PAYLOAD_RAW") - 1, Goridge::PAYLOAD_RAW TSRMLS_CC);
    zend_declare_class_constant_long(frame_ce, "PAYLOAD_ERROR",  sizeof("PAYLOAD_ERROR") - 1, Goridge::PAYLOAD_ERROR TSRMLS_CC);
    zend_declare_class_constant_long(frame_ce, "PAYLOAD_CONTROL",  sizeof("PAYLOAD_CONTROL") - 1, Goridge::PAYLOAD_CONTROL TSRMLS_CC);

    // RELAY
    zend_class_entry _relay_ce;
    INIT_CLASS_ENTRY(_relay_ce, "Spiral\\Goridge\\Relay", relay_methods);

    relay_ce = zend_register_internal_class(&_relay_ce);
    relay_ce->create_object = relay_create;

    memcpy(&relay_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    relay_handlers.offset = XtOffsetOf(relay_object, std);
    relay_handlers.free_obj = relay_free;
    relay_handlers.dtor_obj = zend_objects_destroy_object;

    zend_declare_class_constant_long(relay_ce, "BUFFER_SIZE",  sizeof("BUFFER_SIZE") - 1, Goridge::BUFFER_SIZE TSRMLS_CC);

    // RPC
    zend_class_entry _rpc_ce;
    INIT_CLASS_ENTRY(_rpc_ce, "Spiral\\Goridge\\RPC", rpc_methods);

    rpc_ce = zend_register_internal_class(&_rpc_ce);
    rpc_ce->create_object = rpc_create;

    memcpy(&rpc_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    rpc_handlers.offset = XtOffsetOf(rpc_object, std);
    rpc_handlers.free_obj = rpc_free;
    rpc_handlers.dtor_obj = zend_objects_destroy_object;

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(goridge)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(goridge)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "Goridge support", "enabled");
    php_info_print_table_row(2, "Goridge version", PHP_GORIDGE_VERSION);
    php_info_print_table_end();
}

static const zend_module_dep goridge_module_deps[] = {
    ZEND_MOD_REQUIRED("json")
    ZEND_MOD_END
};

zend_module_entry goridge_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    NULL,
    goridge_module_deps,
    PHP_GORIDGE_EXTNAME,
    NULL,
    PHP_MINIT(goridge),
    PHP_MSHUTDOWN(goridge),
    NULL,
    NULL,
    PHP_MINFO(goridge),
    PHP_GORIDGE_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_GORIDGE
extern "C" {
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(goridge)
}
#endif
