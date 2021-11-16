# Root CMake File
cmake_minimum_required(VERSION 3.10)

# Project
project(cross-capture VERSION 0.1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)

# Include dir
include_directories(include)

if (WIN32)
	set(PLATFORM_DIR "win")
	set(PLATFORM_LIBS gdiplus)
	add_definitions(-D_UNICODE -DUNICODE)
elseif(APPLE)
	set(PLATFORM_DIR "osx")
	set(PLATFORM_LIBS )
endif(WIN32)

# Build library
file(GLOB_RECURSE PLATFORM_LIB_SOURCE_FILES CONFIGURE_DEPENDS lib/platform/${PLATFORM_DIR}/*.cpp)
file(GLOB_RECURSE LIB_SOURCE_FILES CONFIGURE_DEPENDS lib/common/*.cpp)

add_library(core ${LIB_SOURCE_FILES} ${PLATFORM_LIB_SOURCE_FILES})
target_link_libraries(core ${PLATFORM_LIBS})

# Build executable
file(GLOB_RECURSE EXE_SOURCE_FILES COFIGURE_DEPENDS src/*.cpp)

add_executable(cross-capture ${EXE_SOURCE_FILES})
target_link_libraries(cross-capture core)