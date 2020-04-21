cmake_minimum_required(VERSION 3.13)
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()
set (CMAKE_CXX_STANDARD 11)

project(slc)


#The includes
include_directories(include)





add_executable(slc src/error.cpp src/format.cpp src/lexer.cpp src/main.cpp src/os.cpp src/parser.cpp src/statement.cpp src/typing.cpp)