cmake_minimum_required(VERSION 3.13)
project(simplang)
#set(EXECUTABLE_OUTPUT_PATH ~/code/simplang)

#The includes
include_directories(include)

file(GLOB SOURCES src/*.cpp)

#remove from glob
#list(REMOVE_ITEM SOURCES simplang/badcode.cpp)

add_executable(slc ${SOURCES})