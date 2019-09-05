#ifndef PHP_GORIDGE_H
#define PHP_GORIDGE_H

#define PHP_GORIDGE_EXTNAME "goridge"
#define PHP_GORIDGE_VERSION "0.1.0"

#if defined(ZTS) && defined(COMPILE_DL_GORIDGE)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_streams.h"
#include "ext/json/php_json.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "zend_smart_str.h"
}

extern zend_module_entry goridge_module_entry;

#endif
