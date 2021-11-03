cmake_minimum_required(VERSION 3.8)

project(Calculator)

set(SOURCES
        Main.cpp
        Calculator.cpp
		PluginLoader.cpp
)

set(HEADERS
        Calculator.h
		PluginLoader.h
)

add_executable(calculator ${SOURCES} ${HEADERS})

set_target_properties(
    calculator PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)