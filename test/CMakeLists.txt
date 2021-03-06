CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
SET(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/testdata)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doozytestconfig.h.in ${CMAKE_BINARY_DIR}/doozytestconfig.h)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${UTILS_INCLUDE_DIRS}
    ${AUDIO_INCLUDE_DIRS}
    ${UPNPFRAMEWORK_INCLUDE_DIRS}
    ${SERVER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/sqlpp11/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/sqlpp11-connector-sqlite3/include
)

LINK_DIRECTORIES(
    ${AUDIO_LIBRARY_DIRS}
    ${IMAGE_LIBRARY_DIRS}
    ${UPNPFRAMEWORK_LIBRARY_DIRS}
    ${SERVER_LIBRARY_DIRS}
)

SET(LINK_LIBS
    ${AUDIO_LIBRARIES}
    ${IMAGE_LIBRARIES}
    utils
    ${UPNPFRAMEWORK_LIBRARIES}
    ${SERVER_LIBRARIES}
    doozyrenderer
)

SET (TEST_INFRA
    gmock-gtest-all.cpp
    eventnotification.h
    eventlistenermock.h
    settingsmocks.h
    main.cpp
    ../common/settings.cpp
)

IF (UPNP_SERVER)
    SET (SERVER_TESTS
        librarytest.cpp
        librarydatabasetest.cpp
    )
ENDIF ()


ADD_EXECUTABLE(doozytest EXCLUDE_FROM_ALL
    ${TEST_INFRA}
    ${SERVER_TESTS}
)

TARGET_LINK_LIBRARIES(doozytest
	${LINK_LIBS}
)

ADD_TEST(NAME DoozyTests COMMAND doozytest)
ADD_DEPENDENCIES(check doozytest)
