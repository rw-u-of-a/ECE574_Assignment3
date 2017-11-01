# author: Joseph Loubert
# NetID: loubert
# Date: 06 January 2016
# Project Number: 2

# avoid CMake warnings
cmake_minimum_required(VERSION 2.6)

# name of assignment
PROJECT(	dpgen	CXX)

# this file contains more CMakeLists.txt files
ADD_SUBDIRECTORY(	src	)

SET(CMAKE_CXX_FLAGS "-lboost_regex -std=c++11")
