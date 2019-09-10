PHP_ARG_ENABLE([goridge],
  [whether to enable goridge support],
  [AS_HELP_STRING([--enable-goridge],
    [Goridge binary protocol support])],
  [no])

if test "$PHP_GORIDGE" != "no"; then
  PHP_REQUIRE_CXX()                                 \
  PHP_SUBST(GORIDGE_SHARED_LIBADD)                  \
  PHP_NEW_EXTENSION(goridge,                        \
    src/Frame.cpp                                   \
    src/Connection.cpp                              \
    src/StreamConnection.cpp                        \
    src/Relay.cpp                                   \
    src/RPC.cpp                                     \
    goridge.cpp                                     \
  , $ext_shared, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_EXTENSION_DEP(goridge, json)
fi
