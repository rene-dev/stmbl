project(Servoterm)
cmake_minimum_required(VERSION 2.8)

set(SRC_LIST
term.cpp
mathplot.cpp mathplot.h
packet.h
)
include(FindPkgConfig)
list(APPEND PKGDEPS libserialport)
pkg_check_modules(PKGDEPS REQUIRED ${PKGDEPS})
include_directories(${PKGDEPS_STATIC_INCLUDE_DIRS})
link_directories(${PKGDEPS_STATIC_LIBRARY_DIRS})
list(APPEND SERVOTERM_LINK_LIBS ${PKGDEPS_STATIC_LIBRARIES})

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets 3.1 COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

if(APPLE)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_LIST})
else(APPLE)
add_executable(${PROJECT_NAME} ${SRC_LIST})
endif(APPLE)

MESSAGE(STATUS "wxWidgets_LIBRARIES = ${wxWidgets_LIBRARIES}")

list(APPEND SERVOTERM_LINK_LIBS ${wxWidgets_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${SERVOTERM_LINK_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION bin/)

#target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} ${LIBSERIALPORT_LIBRARIES})
