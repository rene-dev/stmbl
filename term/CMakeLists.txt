project(term)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(glm REQUIRED)

set(TERM_INC
	include/mainwindow.hpp
	include/glwidget.hpp
	)

set(TERM_SRC
	src/main.cpp
	src/mainwindow.cpp
	src/glwidget.cpp
	)

set(TERM_UI
	ui/mainwindow.ui
	)

set(TERM_CONF
	config
	)

QT5_WRAP_UI(TERM_UI_INC ${TERM_UI})

add_executable(term ${TERM_INC} ${TERM_SRC} ${TERM_UI_INC})
set_property(TARGET term PROPERTY CXX_STANDARD 14)
set_property(TARGET term PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_options(term PRIVATE -Wall -Wextra)
target_compile_options(term PRIVATE $<$<CONFIG:DEBUG>:-ggdb -O2>)
target_compile_options(term PRIVATE $<$<CONFIG:RELEASE>:-O3 -NDEBUG>)

target_link_libraries(term Qt5::Core Qt5::Widgets)
