
lib_LTLIBRARIES=libml_sockets.la

libml_sockets_la_SOURCES= \
        CFunctions.cpp \
        CSocket.cpp \
        CSocketManager.cpp \
        ml_sockets.cpp \
        CLuaArgument.cpp \
        CLuaArguments.cpp

libml_sockets_la_LIBADD=\
        -L../luaimports \
        -lluaimports  \
        -lpthread \
        -lstdc++ \
        -lm \
        -lz

AM_CPPFLAGS=\
        -I. \
        -I.. \
        -I../luaimports \
        -I../include \
        -DHAVE_ERRNO_AS_DEFINE=1 \
        -pipe
