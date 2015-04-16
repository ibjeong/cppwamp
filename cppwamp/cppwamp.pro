#-------------------------------------------------------------------------------
#               Copyright Butterfly Energy Systems 2014-2015.
#          Distributed under the Boost Software License, Version 1.0.
#             (See accompanying file LICENSE_1_0.txt or copy at
#                   http://www.boost.org/LICENSE_1_0.txt)
#-------------------------------------------------------------------------------

TEMPLATE = lib
CONFIG += plugin precompile_header
CONFIG -= qt

SOURCES += \
    src/cppwamp.cpp

HEADERS += \
    include/cppwamp/asiodefs.hpp \
    include/cppwamp/asyncresult.hpp \
    include/cppwamp/codec.hpp \
    include/cppwamp/connector.hpp \
    include/cppwamp/corosession.hpp \
    include/cppwamp/dialoguedata.hpp \
    include/cppwamp/error.hpp \
    include/cppwamp/json.hpp \
    include/cppwamp/legacytcpconnector.hpp \
    include/cppwamp/legacyudsconnector.hpp \
    include/cppwamp/msgpack.hpp \
    include/cppwamp/null.hpp \
    include/cppwamp/options.hpp \
    include/cppwamp/payload.hpp \
    include/cppwamp/rawsockdefs.hpp \
    include/cppwamp/registration.hpp \
    include/cppwamp/session.hpp \
    include/cppwamp/sessiondata.hpp \
    include/cppwamp/subscription.hpp \
    include/cppwamp/tcpconnector.hpp \
    include/cppwamp/udsconnector.hpp \
    include/cppwamp/variant.hpp \
    include/cppwamp/varianttuple.hpp \
    include/cppwamp/version.hpp \
    include/cppwamp/visitor.hpp \
    include/cppwamp/wampdefs.hpp \
    include/cppwamp/internal/asioconnector.hpp \
    include/cppwamp/internal/asioendpoint.hpp \
    include/cppwamp/internal/asiolistener.hpp \
    include/cppwamp/internal/asiotransport.hpp \
    include/cppwamp/internal/callee.hpp \
    include/cppwamp/internal/client.hpp \
    include/cppwamp/internal/clientinterface.hpp \
    include/cppwamp/internal/config.hpp \
    include/cppwamp/internal/dialogue.hpp \
    include/cppwamp/internal/endian.hpp \
    include/cppwamp/internal/legacyasioendpoint.hpp \
    include/cppwamp/internal/legacyasiotransport.hpp \
    include/cppwamp/internal/messagetraits.hpp \
    include/cppwamp/internal/precompiled.hpp \
    include/cppwamp/internal/rawsockhandshake.hpp \
    include/cppwamp/internal/rawsockheader.hpp \
    include/cppwamp/internal/registrationimpl.hpp \
    include/cppwamp/internal/subscriber.hpp \
    include/cppwamp/internal/subscriptionimpl.hpp \
    include/cppwamp/internal/tcpacceptor.hpp \
    include/cppwamp/internal/tcpopener.hpp \
    include/cppwamp/internal/udsacceptor.hpp \
    include/cppwamp/internal/udsopener.hpp \
    include/cppwamp/internal/variantbuilder.hpp \
    include/cppwamp/internal/varianttraits.hpp \
    include/cppwamp/internal/varianttraitsfwd.hpp \
    include/cppwamp/internal/variantvisitors.hpp \
    include/cppwamp/internal/wampmessage.hpp \
    \
    include/cppwamp/internal/asyncresult.ipp \
    include/cppwamp/internal/corosession.ipp \
    include/cppwamp/internal/dialoguedata.ipp \
    include/cppwamp/internal/endian.ipp \
    include/cppwamp/internal/error.ipp \
    include/cppwamp/internal/json.ipp \
    include/cppwamp/internal/legacytcpconnector.ipp \
    include/cppwamp/internal/legacyudsconnector.ipp \
    include/cppwamp/internal/messagetraits.ipp \
    include/cppwamp/internal/msgpack.ipp \
    include/cppwamp/internal/null.ipp \
    include/cppwamp/internal/options.ipp \
    include/cppwamp/internal/passkey.hpp \
    include/cppwamp/internal/payload.ipp \
    include/cppwamp/internal/registration.ipp \
    include/cppwamp/internal/session.ipp \
    include/cppwamp/internal/sessiondata.ipp \
    include/cppwamp/internal/subscription.ipp \
    include/cppwamp/internal/tcpconnector.ipp \
    include/cppwamp/internal/udsconnector.ipp \
    include/cppwamp/internal/variant.ipp \
    include/cppwamp/internal/varianttuple.ipp \
    include/cppwamp/internal/version.ipp \
    include/cppwamp/internal/visitor.ipp

OTHER_FILES += \
    CMakeLists.txt

PRECOMPILED_HEADER = include/cppwamp/internal/precompiled.hpp

#Preprocessor macros
DEFINES += CPPWAMP_COMPILED_LIB

#Enable C++11 support
QMAKE_CXXFLAGS += -std=c++11

#Enable support for threads
QMAKE_CXXFLAGS += -pthread

#Stop compiling at first error
#QMAKE_CXXFLAGS += -Wfatal-errors

#Add debugging symbols if in debug configuration
CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g
    QMAKE_LFLAGS += -g
}

#Add include search directories
INCLUDEPATH += \
    $$PWD/include \
    $$PWD/../ext/boost \
    $$PWD/../ext/endian/include \
    $$PWD/../ext/msgpack-c/include \
    $$PWD/../ext/rapidjson/include

#These are to suppress warnings from library headers
QMAKE_CXXFLAGS += \
    -isystem $$PWD/../ext/boost \
    -isystem $$PWD/../ext/msgpack-c/include

LIBS += -pthread

DEPENDPATH += ./include ./include/internal