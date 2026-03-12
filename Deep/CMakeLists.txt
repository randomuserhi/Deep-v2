# TODO(randomuserhi): Migrate to just use clang (remove MSVC entirely)
#                     clang-cl should be supported -> still bind to propert runtime and stuff on windows

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Deep VERSION 0.0.1 LANGUAGES CXX)

if (NOT DEFINED RELEASE_BUILD)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(STATUS "Configuring for Debug build")
		set(_USE_ASSERTS ON)
		set(RELEASE_BUILD OFF)
		set(GENERATE_DEBUG_SYMBOLS ON)
	elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
		message(STATUS "Configuring for Release build")
		set(_USE_ASSERTS OFF)
		set(RELEASE_BUILD ON)
		set(GENERATE_DEBUG_SYMBOLS OFF)
	elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		message(STATUS "Configuring for Release build (With Debug Info)")
		set(_USE_ASSERTS ON)
		set(RELEASE_BUILD ON)
		set(GENERATE_DEBUG_SYMBOLS ON)
	else()
		message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
	endif()
endif()

# When turning this option on, the library will be compiled as a shared library.
option(BUILD_SHARED_LIBS "Compile Deep as a shared library" OFF)

# When turning this option on, the library will be compiled using assertions. By default asserts are enabled in Debug build.
option(USE_ASSERTS "Enable asserts" ${_USE_ASSERTS})

# When turning this on, the library will be compiled with C++ exceptions enabled.
option(CPP_EXCEPTIONS_ENABLED "Enable C++ exceptions" OFF)

# When turning this on, the library will not use any SIMD intrinsics
option(NO_SIMD_INTRINSICS "Disable SIMD intrinsics" OFF)

# When turning this on, the library will not use type-punning tricks to abide by strict-aliasing rules and fallback to either
# memcpy or bitcast to avoid strict aliasing issues.
option(USE_STRICT_ALIASING "Abide by strict aliasing rules" OFF)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
option(USE_SSE4_1 "Enable SSE4.1" ON)
option(USE_SSE4_2 "Enable SSE4.2" ON)
option(USE_AVX "Enable AVX" ON)
option(USE_AVX2 "Enable AVX2" ON)
option(USE_AVX512 "Enable AVX512" OFF)
option(USE_LZCNT "Enable LZCNT" ON)
option(USE_TZCNT "Enable TZCNT" ON)
option(USE_F16C "Enable F16C" ON)
option(USE_FMADD "Enable FMADD" ON)

include(CMakeDependentOption)

# When turning this option on, the library will be compiled with interprocedural optimizations enabled, also known as link-time optimizations or link-time code generation.
# Note that if you turn this on you need to use SET_INTERPROCEDURAL_OPTIMIZATION() or set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON) to enable LTO specifically for your own project as well.
# If you don't do this you may get an error: /usr/bin/ld: libDeep.a: error adding symbols: file format not recognized
cmake_dependent_option(INTERPROCEDURAL_OPTIMIZATION "Enable interprocedural optimizations" ON "RELEASE_BUILD" OFF)

# Ability to toggle between the static and DLL versions of the MSVC runtime library
# Windows Store only supports the DLL version
cmake_dependent_option(USE_STATIC_MSVC_RUNTIME_LIBRARY "Use the static MSVC runtime library" ON "MSVC;NOT WINDOWS_STORE" OFF)

# This library supports C++20 and higher, By default will use standard C++20, but you may specify a higher one.
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
elseif (CMAKE_CXX_STANDARD LESS 20)
	message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 20. Provided: ${CMAKE_CXX_STANDARD}")
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
	if (INTERPROCEDURAL_OPTIMIZATION
		AND NOT (BUILD_SHARED_LIBS))
		include(CheckIPOSupported)
		check_ipo_supported(RESULT IS_IPO_SUPPORTED OUTPUT IPO_CHECK_OUTPUT)

		if (IS_IPO_SUPPORTED)
			message("Interprocedural optimizations are turned on")
			set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON PARENT_SCOPE)
			set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DISTRIBUTION ON PARENT_SCOPE)
		else()
			message("Warning: Interprocedural optimizations are not supported for this target, turn off the option INTERPROCEDURAL_OPTIMIZATION to disable this warning")
		endif()
	endif()
endfunction()
SET_INTERPROCEDURAL_OPTIMIZATION()

# Root location
set(DEEP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Obtain source files
file(GLOB_RECURSE DEEP_SRC_FILES CONFIGURE_DEPENDS "${DEEP_ROOT}/src/*.cpp")
file(GLOB_RECURSE DEEP_HEADER_FILES CONFIGURE_DEPENDS "${DEEP_ROOT}/public/*.h")
file(GLOB_RECURSE DEEP_INL_FILES CONFIGURE_DEPENDS "${DEEP_ROOT}/public/*.inl")

# Create Deep lib
add_library(Deep ${DEEP_SRC_FILES} ${DEEP_HEADER_FILES} ${DEEP_INL_FILES})

# Add include directory
target_include_directories(Deep PUBLIC "${DEEP_ROOT}/public")

# Handle shared library compilation
if (BUILD_SHARED_LIBS)
	# Set default visibility to hidden
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)

	# Public define to instruct user code to import Core symbols (rather than use static linking)
	target_compile_definitions(Deep PUBLIC DEEP_SHARED_LIBRARY)

	# Private define to instruct the library to export symbols for shared linking
	target_compile_definitions(Deep PRIVATE DEEP_BUILD_SHARED_LIBRARY)
endif()

# Applies to clang-cl and MSVC
if (MSVC)
	# Disable safe versions of functions
	target_compile_definitions(Deep PUBLIC _CRT_SECURE_NO_WARNINGS)

	# Set runtime library
	if (USE_STATIC_MSVC_RUNTIME_LIBRARY)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	target_compile_options(Deep PRIVATE 
		/Zc:__cplusplus
		/nologo
		/FC
		)
endif()

# Applies to clang-cl and clang
if (CLANG)
	# Disable unnecessary warnings
	target_compile_options(Deep PRIVATE 
		-Wno-c++98-compat
		-Wno-c++98-compat-pedantic 
		-Wno-unused-macros
		)
endif()

if (MSVC)
	message(FATAL_ERROR "Unsupported compiler")
elseif (CLANG)
	# Disable floating point exceptions
	target_compile_options(Deep PRIVATE -fno-trapping-math)

	# Enable warnings
	target_compile_options(Deep PRIVATE -Wall -Werror)

	# Optionally generate debug symbols
	if (GENERATE_DEBUG_SYMBOLS)
		target_compile_options(Deep PRIVATE -g)
	endif()

	# Enable/Disable exceptions
	message(STATUS "CPP_EXCEPTIONS_ENABLED: ${CPP_EXCEPTIONS_ENABLED}")
	if (NOT CPP_EXCEPTIONS_ENABLED)
		target_compile_options(Deep PRIVATE -fno-exceptions)
	else()
		target_compile_options(Deep PRIVATE -fexceptions)
	endif()

	# Remove buffer protections in release mode
	target_compile_options(Deep PRIVATE
		$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-fno-stack-protector>
	)
else()
	message(FATAL_ERROR "Unsupported compiler")
endif()

# Emit Strict Aliasing
if (USE_STRICT_ALIASING)
	target_compile_definitions(Deep PUBLIC DEEP_USE_STRICT_ALIASING)
endif()

# Emit Asserts
if (USE_ASSERTS)
	target_compile_definitions(Deep PUBLIC DEEP_USE_ASSERTS)
endif()

# Emit C Bindings
if (USE_C_BINDINGS)
	target_compile_definitions(Deep PUBLIC DEEP_USE_C_BINDINGS)
endif()

# Emit the instruction set definitions to ensure that child projects use the same settings even if they override the used instruction sets (a mismatch causes link errors)
function(EMIT_X86_INSTRUCTION_SET_DEFINITIONS)
	if (USE_AVX512)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX512)
	endif()
	if (USE_AVX2)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX2)
	endif()
	if (USE_AVX)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX)
	endif()
	if (USE_SSE4_1)
		target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_1)
	endif()
	if (USE_SSE4_2)
		target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_2)
	endif()
	if (USE_LZCNT)
		target_compile_definitions(Deep PUBLIC DEEP_USE_LZCNT)
	endif()
	if (USE_TZCNT)
		target_compile_definitions(Deep PUBLIC DEEP_USE_TZCNT)
	endif()
	if (USE_F16C)
		target_compile_definitions(Deep PUBLIC DEEP_USE_F16C)
	endif()
	if (USE_FMADD)
		target_compile_definitions(Deep PUBLIC DEEP_USE_FMADD)
	endif()
endfunction()

# Add the compiler commandline flags to select the right instruction sets
if (NOT NO_SIMD_INTRINSICS)
	message(STATUS "Using SIMD Intrinsics")

	if (MSVC)
		if (USE_AVX512)
			target_compile_options(Deep PUBLIC /arch:AVX512)
		elseif (USE_AVX2)
			target_compile_options(Deep PUBLIC /arch:AVX2)
		elseif (USE_AVX)
			target_compile_options(Deep PUBLIC /arch:AVX)
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	elseif (CLANG)
		if (USE_AVX512)
			target_compile_options(Deep PUBLIC -mavx512f -mavx512cd -mavx512bw -mavx512dq -mavx512vl)
		elseif (USE_AVX2)
			target_compile_options(Deep PUBLIC -mavx2)
		elseif (USE_AVX)
			target_compile_options(Deep PUBLIC -mavx)
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	else()
		# TODO(randomuserhi)
		message(FATAL_ERROR "Unsupported compiler")
	endif()
else()
	target_compile_definitions(Deep PUBLIC DEEP_DONT_USE_SIMD_INTRINSICS)
endif()