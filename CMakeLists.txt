# author: Joseph Loubert
# NetID: loubert
# Date: 22 November 2017
# Project Number: 3

# avoid CMake warnings
cmake_minimum_required(VERSION 2.6)

# name of assignment
PROJECT(	hlsyn	CXX)

# this file contains more CMakeLists.txt files
ADD_SUBDIRECTORY(	src	)

# uncomment following line if running on ECE3
SET(CMAKE_CXX_FLAGS "-lboost_regex -std=c++11")

# comment following line if running on ECE3
SET(CMAKE_CXX_FLAGS "-std=c++11")