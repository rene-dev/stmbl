project(Servoterm)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../term/cmake/Modules/")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGLExtensions REQUIRED)
find_package(Qt5SerialPort REQUIRED)

set(TERM_INC
	include/mainwindow.hpp
	include/glwidget.hpp
	include/functiongraph.hpp
	)

set(TERM_SRC
	src/main.cpp
	src/mainwindow.cpp
	src/glwidget.cpp
	src/functiongraph.cpp
	)

set(TERM_UI
	ui/mainwindow.ui
	)

set(TERM_CONF
	config
	)

QT5_WRAP_UI(TERM_UI_INC ${TERM_UI})

if(APPLE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../term/resources/stmbl.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${TERM_INC} ${TERM_SRC} ${TERM_UI_INC} ${CMAKE_CURRENT_SOURCE_DIR}/../term/resources/stmbl.icns)
else(APPLE)
add_executable(${PROJECT_NAME} ${TERM_INC} ${TERM_SRC} ${TERM_UI_INC})
endif(APPLE)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-ggdb -O2>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:-O3 -NDEBUG>)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::OpenGLExtensions Qt5::SerialPort)
