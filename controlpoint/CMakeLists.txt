CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(THRIFT_PKG thrift)
FIND_PATH(THRIFT_INCLUDE_DIRS
    NAMES thrift/Thrift.h
    PATHS ${THRIFT_PKG_INCLUDE_DIRS}
)

FIND_LIBRARY(THRIFT_LIBRARIES
    NAMES ${THRIFT_PKG_LIBRARIES}
    HINTS ${THRIFT_PKG_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UTILS_INCLUDE_DIRS}
    ${UPNPFRAMEWORK_INCLUDE_DIRS}
    ${THRIFT_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${UTILS_LIBRARY_DIRS}
    ${UPNPFRAMEWORK_LIBRARY_DIRS}
    ${THRIFT_LIBRARY_DIRS}
)

SET(LINK_LIBS
    ${UTILS_LIBRARIES}
    ${UPNPFRAMEWORK_LIBRARIES}
    ${THRIFT_LIBRARIES}
    ${THRIFT_LIBRARIES}
)

ADD_EXECUTABLE(doozycontrolpoint
    gen-cpp/controlpoint_constants.h gen-cpp/controlpoint_constants.cpp
    gen-cpp/controlpoint_types.h gen-cpp/controlpoint_types.cpp
    gen-cpp/ControlPoint.h gen-cpp/ControlPoint.cpp
    doozycontrolpoint.h doozycontrolpoint.cpp
    main.cpp
    controlpoint.thrift
)

SOURCE_GROUP(generated REGULAR_EXPRESSION gen-cpp/)

TARGET_LINK_LIBRARIES(doozycontrolpoint ${LINK_LIBS})
INSTALL(TARGETS doozycontrolpoint RUNTIME DESTINATION bin)

