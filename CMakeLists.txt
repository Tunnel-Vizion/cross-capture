# Root CMake File
cmake_minimum_required(VERSION 3.10)

# Project
project(cross-capture VERSION 0.1.0 LANGUAGES CXX)

# Settings
option(CROSS_CAPTURE_BUILD_CLI "Build Command Line Tool" ON)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)

# Include dir
include_directories(include)

if(WIN32)
	set(PLATFORM_DIR "win")
	set(PLATFORM_LIBS gdiplus)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
elseif(APPLE)
	set(PLATFORM_DIR "osx")

	find_library(APP_SERVICES_LIB ApplicationServices)
	set(PLATFORM_LIBS ${APP_SERVICES_LIB})

	if (CROSS_CAPTURE_BUILD_CLI)
		add_definitions(-DOSX_REQUEST_CAPTURE_ACCESS)
	endif(CROSS_CAPTURE_BUILD_CLI)
endif()

# Build library
file(GLOB_RECURSE PLATFORM_LIB_SOURCE_FILES CONFIGURE_DEPENDS lib/platform/${PLATFORM_DIR}/*.cpp)
file(GLOB_RECURSE LIB_SOURCE_FILES CONFIGURE_DEPENDS lib/common/*.cpp)

add_library(cc-lib ${LIB_SOURCE_FILES} ${PLATFORM_LIB_SOURCE_FILES})
target_link_libraries(cc-lib ${PLATFORM_LIBS})

# Build executable
if(CROSS_CAPTURE_BUILD_CLI)
	file(GLOB_RECURSE EXE_SOURCE_FILES COFIGURE_DEPENDS src/*.cpp)

	add_executable(cc-cli ${EXE_SOURCE_FILES})
	target_link_libraries(cc-cli cc-lib)
endif()