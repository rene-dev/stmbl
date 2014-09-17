project(Servoterm)
cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)
list(APPEND PKGDEPS libserialport)
#find_package(PkgConfig)
pkg_check_modules(PKGDEPS REQUIRED ${PKGDEPS})
include_directories(${PKGDEPS_STATIC_INCLUDE_DIRS})
link_directories(${PKGDEPS_STATIC_LIBRARY_DIRS})
list(APPEND SERVOTERM_LINK_LIBS ${PKGDEPS_STATIC_LIBRARIES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
add_definitions("-std=c++11")
add_definitions("-DSRCDIR=\"${PROJECT_SOURCE_DIR}\"")
add_definitions("-D__WXDEBUG__")
###find_package(libserialport REQUIRED)

set(SRC_LIST
main.cpp
mainframe.cpp mainframe.hpp
)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets 3.0 COMPONENTS core base REQUIRED)
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
