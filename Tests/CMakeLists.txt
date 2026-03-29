# TODO(randomuserhi): Migrate to just use clang (remove MSVC entirely)
#                     clang-cl should be supported -> still bind to propert runtime and stuff on windows

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(Deep_Tests VERSION 0.0.1 LANGUAGES CXX)

set(MESSAGE_PREFIX "[${PROJECT_NAME}]")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Debug build")
	set(RELEASE_BUILD OFF)
	set(GENERATE_DEBUG_SYMBOLS ON)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Release build")
	set(RELEASE_BUILD ON)
	set(GENERATE_DEBUG_SYMBOLS OFF)
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Release build (With Debug Info)")
	set(RELEASE_BUILD ON)
	set(GENERATE_DEBUG_SYMBOLS ON)
else()
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

# When turning this option on, the tests will be ran against Deep with dynamic linking instead of static linking.
option(USE_DYNAMIC_LINKING "Use dynamic linking with Deep or static linking" ON)

include(CMakeDependentOption)

# When turning this option on, the library will be compiled with interprocedural optimizations enabled, also known as link-time optimizations or link-time code generation.
# Note that if you turn this on you need to use SET_INTERPROCEDURAL_OPTIMIZATION() or set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON) to enable LTO specifically for your own project as well.
# If you don't do this you may get an error: /usr/bin/ld: libDeep.a: error adding symbols: file format not recognized
cmake_dependent_option(INTERPROCEDURAL_OPTIMIZATION "Enable interprocedural optimizations" ON "RELEASE_BUILD" OFF)

# Ability to toggle between the static and DLL versions of the MSVC runtime library
# Windows Store only supports the DLL version
cmake_dependent_option(USE_STATIC_MSVC_RUNTIME_LIBRARY "Use the static MSVC runtime library" ON "MSVC" OFF)

# This library supports C++20 and higher, By default will use standard C++20, but you may specify a higher one.
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
elseif (CMAKE_CXX_STANDARD LESS 20)
	message(FATAL_ERROR "${MESSAGE_PREFIX} CMAKE_CXX_STANDARD must be at least 20. Provided: ${CMAKE_CXX_STANDARD}")
endif()

# Detect compiler
set(CLANG FALSE)
set(GCC FALSE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GCC TRUE)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/shared_libs")

# Enable link time optimization in Release and Distribution mode if requested and available
function(SET_INTERPROCEDURAL_OPTIMIZATION)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF PARENT_SCOPE)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION OFF PARENT_SCOPE)

	# When compiling as a shared lib, turning on LTO causes errors of the form 'ld.exe: cannot export symbol X wrong type (4 vs 3)'
	if (INTERPROCEDURAL_OPTIMIZATION)
		if (NOT (BUILD_SHARED_LIBS))
			include(CheckIPOSupported)
			check_ipo_supported(RESULT IS_IPO_SUPPORTED OUTPUT IPO_CHECK_OUTPUT)

			if (IS_IPO_SUPPORTED)
				message(STATUS "${MESSAGE_PREFIX} Interprocedural optimizations are turned on")
				set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON PARENT_SCOPE)
				set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION ON PARENT_SCOPE)
			else()
				message(WARNING "${MESSAGE_PREFIX} Interprocedural optimizations are not supported for this target, turn off the option INTERPROCEDURAL_OPTIMIZATION to disable this warning")
			endif()
		else()
			message(WARNING "${MESSAGE_PREFIX} Interprocedural optimizations do not play nice with shared libraries, and thus is ignored. Turn off the option INTERPROCEDURAL_OPTIMIZATION to disable this warning")
		endif()
	endif()
endfunction()
SET_INTERPROCEDURAL_OPTIMIZATION()

set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(DEEP_ROOT "${REPO_ROOT}/Deep")

# Add Deep to project
set(_TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ${USE_DYNAMIC_LINKING} CACHE BOOL "Force dynamic build for Deep" FORCE)
add_subdirectory("${DEEP_ROOT}" "${CMAKE_BINARY_DIR}/thirdparty/Deep")
set(BUILD_SHARED_LIBS ${_TEMP_BUILD_SHARED_LIBS} CACHE BOOL "Restore global BUILD_SHARED_LIBS" FORCE)

# Root location
set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Obtain source files
file(GLOB_RECURSE TEST_SRC_FILES CONFIGURE_DEPENDS "${TEST_ROOT}/src/*.cpp")

# Test executable
add_executable(Tests ${TEST_SRC_FILES})

target_include_directories(Tests PUBLIC "${TEST_ROOT}/src")

# Add includes and link Deep
target_include_directories(Tests PRIVATE "${DEEP_ROOT}/public")
target_link_libraries(Tests PRIVATE Deep)

# Applies to clang-cl and MSVC
if (MSVC)
	# Disable safe versions of functions
	target_compile_definitions(Tests PUBLIC _CRT_SECURE_NO_WARNINGS)

	# Set runtime library
	if (USE_STATIC_MSVC_RUNTIME_LIBRARY)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	target_compile_options(Tests PRIVATE 
		/Zc:__cplusplus
		/nologo
		/FC
		)
endif()

# Applies to clang-cl and clang
if (CLANG)
	# Disable unnecessary warnings
	target_compile_options(Tests PRIVATE 
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic 
		-Wno-unused-macros
		)
endif()

if (MSVC)
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
elseif (CLANG)
	# Enable warnings
	target_compile_options(Tests PRIVATE 
		-Wall
		-Wextra
		-Werror
		)	

	# Extra useful warnings
	target_compile_options(Tests PRIVATE
		-Wpessimizing-move
		-Wredundant-move
		-Wmove
		-Wreturn-std-move
		-Wdefaulted-function-deleted
		-Wself-move
		)

	# Disable RTTI
	message(STATUS "${MESSAGE_PREFIX} USE_RTTI: ${USE_RTTI}")
	if (NOT USE_RTTI)
		target_compile_options(Tests PRIVATE -fno-rtti)
	endif()

	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		target_compile_options(Tests PRIVATE -g)
	endif()

	# Enable/Disable exceptions
	message(STATUS "${MESSAGE_PREFIX} CPP_EXCEPTIONS_ENABLED: ${CPP_EXCEPTIONS_ENABLED}")
	if (NOT CPP_EXCEPTIONS_ENABLED)
		target_compile_options(Tests PRIVATE -fno-exceptions)
	else()
		target_compile_options(Tests PRIVATE -fexceptions)
	endif()
else()
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
endif()
