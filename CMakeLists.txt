﻿cmake_minimum_required(VERSION 3.12)
set(CMAKE_SUPPRESS_REGENERATION TRUE CACHE BOOL "Disable Zero Check Project")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PROJ_NAME "AI")
project(${PROJ_NAME})

include(Common.cmake)

add_subdirectory(BehaviorTree)
add_subdirectory(Perception)

