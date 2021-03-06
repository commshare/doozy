CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)
ENABLE_TESTING()

PROJECT(doozy)
SET(PACKAGE "doozy")
SET(PACKAGE_NAME "Doozy")
SET(PACKAGE_VERSION_MAJOR 0)
SET(PACKAGE_VERSION_MINOR 0)
SET(PACKAGE_VERSION_PATCH 1)
SET(PACKAGE_LOCALE_DIR ${CMAKE_INSTALL_PREFIX}/share/locale)

SET(CPACK_PACKAGE_NAME ${PACKAGE})
SET(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION_PATCH})
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_PACKAGE_FILE_NAME $CPACK_PACKAGE_NAME)
SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-bin")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_IGNORE_FILES "/build/;/debug/;/release/;/dist/;/gfx/;/include/;/lib/;/web/;/.git/;/server/;/renderer/;/controlpoint/;/tags;~$;${CPACK_SOURCE_IGNORE_FILES}")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY(gejengel)
#SET(CPACK_RESOURCE_FILE_LICENSE(COPYING)
#SET(CPACK_RESOURCE_FILE_README(README)
#SET(CPACK_MONOLITHIC_INSTALL()
INCLUDE(CPack)

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)

OPTION(UPNP_RENDERER "Renderer device support" ON)
OPTION(UPNP_SERVER "Server device support" OFF)
OPTION(UPNP_CONTROLPOINT "Controlpoint device support" OFF)
OPTION(STATIC_BINARY "Build static binary" OFF)
OPTION(SHARED_LIBRARY "Create single shared library containing all dependencies" OFF)

IF (STATIC_BINARY)
    ADD_DEFINITIONS("-DAL_LIBTYPE_STATIC")
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Gcc")
        SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    ENDIF ()
ENDIF ()

ADD_DEFINITIONS("-std=c++1y -D_FILE_OFFSET_BITS=64")
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND})
ADD_CUSTOM_TARGET(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/CMakeUninstall.cmake)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

FIND_PACKAGE(PkgConfig)

ADD_SUBDIRECTORY(modules/utils)
GET_DIRECTORY_PROPERTY(UTILS_INCLUDE_DIRS	DIRECTORY modules/utils INCLUDE_DIRECTORIES)

ADD_SUBDIRECTORY(modules/audio)
GET_DIRECTORY_PROPERTY(AUDIO_INCLUDE_DIRS	DIRECTORY modules/audio INCLUDE_DIRECTORIES)

ADD_SUBDIRECTORY(modules/image)
GET_DIRECTORY_PROPERTY(IMAGE_INCLUDE_DIRS	DIRECTORY modules/image INCLUDE_DIRECTORIES)

ADD_SUBDIRECTORY(modules/upnp)
GET_DIRECTORY_PROPERTY(UPNPFRAMEWORK_INCLUDE_DIRS	DIRECTORY modules/upnp INCLUDE_DIRECTORIES)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doozyconfig.h.in ${CMAKE_BINARY_DIR}/doozyconfig.h)

IF (UPNP_RENDERER)
    ADD_SUBDIRECTORY(renderer)
    GET_DIRECTORY_PROPERTY(RENDERER_LIBRARY_DIRS  DIRECTORY renderer LINK_DIRECTORIES)
    GET_DIRECTORY_PROPERTY(RENDERER_LIBRARIES     DIRECTORY renderer DEFINITION RENDERER_LIBRARIES)
ENDIF ()

IF (UPNP_CONTROLPOINT)
    ADD_SUBDIRECTORY(controlpoint)
ENDIF ()

IF (UPNP_SERVER)
    ADD_SUBDIRECTORY(modules/sqlpp11)
    ADD_SUBDIRECTORY(modules/sqlpp11-connector-sqlite3)

    ADD_SUBDIRECTORY(server)
    GET_DIRECTORY_PROPERTY(SERVER_LIBRARY_DIRS 	DIRECTORY server LINK_DIRECTORIES)
    GET_DIRECTORY_PROPERTY(SERVER_LIBRARIES 	DIRECTORY server DEFINITION SERVER_LIBRARIES)
ENDIF ()

ADD_SUBDIRECTORY(test)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UTILS_INCLUDE_DIRS}
    ${UPNPFRAMEWORK_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(LINK_LIBS
    ${RENDERER_LIBRARIES}
    ${SERVER_LIBRARIES}
    image
    audio
    upnpframework
    utils
)

LIST(REVERSE LINK_LIBS)
LIST(REMOVE_DUPLICATES LINK_LIBS)
LIST(REVERSE LINK_LIBS)

SET(DOOZY_SOURCES
    common/doozydeviceinterface.h
    common/doozydevicefactory.h common/doozydevicefactory.cpp
    common/settings.h common/settings.cpp
    common/devicedescriptions.h
)

ADD_EXECUTABLE(doozy
    main.cpp
    ${DOOZY_SOURCES}
)

TARGET_LINK_LIBRARIES(doozy ${LINK_LIBS})
INSTALL(TARGETS doozy RUNTIME DESTINATION bin)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/CMakeUninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/CMakeUninstall.cmake" IMMEDIATE @ONLY)

IF (SHARED_LIBRARY)
    ADD_LIBRARY(doozylib SHARED ${DOOZY_SOURCES})
    TARGET_LINK_LIBRARIES(doozylib "-Wl,--whole-archive" ${LINK_LIBS} "-Wl,--no-whole-archive")
ENDIF ()
