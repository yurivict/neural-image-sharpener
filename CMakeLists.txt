# Copyright (C) 2020 by Yuri Victorovich. All rights reserved.


cmake_minimum_required(VERSION 3.1.0)

project(neural-image-sharpener)


##
## C++ standard that we use is the highest possible
##
set(CMAKE_CXX_FLAGS "-std=c++2a -Wall")

##
## Build type
##
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

##
## Some required locations
##
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

##
## Find the required dependencies
##
find_package(png++ REQUIRED)
find_package(PNG REQUIRED)

include_directories(${png++_INCLUDE_DIRS})

##
## Targets
##
add_executable(nn-train-image-sharpener
	train.cpp
	training-data.cpp
)
add_executable(nn-sharpen-image
	sharpen.cpp
)
target_link_libraries(nn-train-image-sharpener
	${png++_LIBRARIES}
)
target_link_libraries(nn-sharpen-image
	${png++_LIBRARIES}
)

##
## Install targets
##

install(TARGETS nn-train-image-sharpener DESTINATION bin)
install(TARGETS nn-sharpen-image DESTINATION bin)
