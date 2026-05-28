# TODO(randomuserhi): Migrate to just use clang (remove MSVC entirely)
#                     clang-cl should be supported -> still bind to propert runtime and stuff on windows

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(Deep VERSION 0.0.1 LANGUAGES CXX)

set(MESSAGE_PREFIX "[${PROJECT_NAME}]")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Debug build")
	set(DEEP_USE_ASSERTS_DEFAULT ON)
	set(RELEASE_BUILD OFF)
	set(GENERATE_DEBUG_SYMBOLS ON)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Release build")
	set(DEEP_USE_ASSERTS_DEFAULT OFF)
	set(RELEASE_BUILD ON)
	set(GENERATE_DEBUG_SYMBOLS OFF)
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	message(STATUS "${MESSAGE_PREFIX} Configuring for Release build (With Debug Info)")
	set(DEEP_USE_ASSERTS_DEFAULT ON)
	set(RELEASE_BUILD ON)
	set(GENERATE_DEBUG_SYMBOLS ON)
else()
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

# When turning this option on, the library will be compiled as a shared library.
# Uses the value of BUILD_SHARED_LIBS if provided
option(DEEP_BUILD_SHARED_LIBS "Compile Deep as a shared library" OFF)
if (DEFINED BUILD_SHARED_LIBS)
	set(DEEP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
endif()

# When turning this option on, the library will be compiled using assertions. By default asserts are enabled in Debug build.
option(DEEP_USE_ASSERTS "Enable asserts" ${DEEP_USE_ASSERTS_DEFAULT})

# When turning this on, the library will be compiled with C++ exceptions enabled.
option(DEEP_CPP_EXCEPTIONS_ENABLED "Enable C++ exceptions" OFF)

# When turning this on, the library will not use any SIMD intrinsics
option(DEEP_NO_SIMD_INTRINSICS "Disable SIMD intrinsics" OFF)

# When turning this on, the library will not use type-punning tricks to abide by strict-aliasing rules and fallback to either
# memcpy or bitcast to avoid strict aliasing issues.
option(DEEP_USE_STRICT_ALIASING "Abide by strict aliasing rules" OFF)

# When turned on, the library will be compiled with RTTI
option(DEEP_USE_RTTI "Enable RTTI" OFF)

option(DEEP_USE_WASM_SIMD "Enable WASM_SIMD" OFF)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
option(DEEP_USE_SSE4_1 "Enable SSE4.1" ON)
option(DEEP_USE_SSE4_2 "Enable SSE4.2" ON)
option(DEEP_USE_AVX "Enable AVX" ON)
option(DEEP_USE_AVX2 "Enable AVX2" ON)
option(DEEP_USE_AVX512 "Enable AVX512" OFF)
option(DEEP_USE_LZCNT "Enable LZCNT" ON)
option(DEEP_USE_TZCNT "Enable TZCNT" ON)
option(DEEP_USE_F16C "Enable F16C" ON)
option(DEEP_USE_FMADD "Enable FMADD" ON)

option(DEEP_USE_C_BINDINGS "Enable C Bindings" OFF)

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
				message(STATUS "${IPO_CHECK_OUTPUT}")
			endif()
		else()
			message(WARNING "${MESSAGE_PREFIX} Interprocedural optimizations do not play nice with shared libraries, and thus is ignored.")
		endif()
	endif()
endfunction()
SET_INTERPROCEDURAL_OPTIMIZATION()

# Root location
set(DEEP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Obtain source files
file(GLOB_RECURSE DEEP_SRC_FILES CONFIGURE_DEPENDS "${DEEP_ROOT}/src/*.cpp")

# Create Deep lib
if (DEEP_BUILD_SHARED_LIBS)
	add_library(Deep SHARED ${DEEP_SRC_FILES})
else()
	add_library(Deep STATIC ${DEEP_SRC_FILES})
endif()

# Add include directory
target_include_directories(Deep PUBLIC "${DEEP_ROOT}/public")

# Handle shared library compilation
if (DEEP_BUILD_SHARED_LIBS)
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
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
elseif (CLANG)
	# Disable floating point exceptions
	target_compile_options(Deep PRIVATE -fno-trapping-math)

	# Enable warnings
	target_compile_options(Deep PRIVATE -Wall -Wextra -Werror)

	# Extra useful warnings
	target_compile_options(Deep PRIVATE
		-Wpessimizing-move
		-Wredundant-move
		-Wmove
		-Wreturn-std-move
		-Wdefaulted-function-deleted
		-Wself-move
		)

	# Optionally generate debug symbols
	message(STATUS "${MESSAGE_PREFIX} GENERATE_DEBUG_SYMBOLS: ${GENERATE_DEBUG_SYMBOLS}")
	if (GENERATE_DEBUG_SYMBOLS)
		target_compile_options(Deep PRIVATE -g)

		# Generate pdb files for windows debugging
		if (WIN32)
			include(CheckLinkerFlag)
			check_linker_flag(CXX "-fuse-ld=lld" HAS_LLD)
			if(HAS_LLD)
				target_compile_options(Engine PRIVATE -gcodeview)
				target_link_options(Engine PRIVATE -fuse-ld=lld -Wl,/debug)
			else()
				check_linker_flag(CXX "-fuse-ld=link" HAS_LINK)
				if (HAS_LINK)
					target_compile_options(Engine PRIVATE -gcodeview)
					target_link_options(Engine PRIVATE -fuse-ld=link -Wl,/debug)
				else()
					message(WARNING "lld or link not available, skipping PDB generation.")
				endif()
			endif()
		endif()
	endif()

	# Disable RTTI
	message(STATUS "${MESSAGE_PREFIX} USE_RTTI: ${DEEP_USE_RTTI}")
	if (NOT DEEP_USE_RTTI)
		target_compile_options(Deep PRIVATE -fno-rtti)
	endif()

	# Enable/Disable exceptions
	message(STATUS "${MESSAGE_PREFIX} CPP_EXCEPTIONS_ENABLED: ${DEEP_CPP_EXCEPTIONS_ENABLED}")
	if (NOT DEEP_CPP_EXCEPTIONS_ENABLED)
		target_compile_options(Deep PRIVATE -fno-exceptions)
	else()
		target_compile_options(Deep PRIVATE -fexceptions)
	endif()

	# Remove buffer protections in release mode
	target_compile_options(Deep PRIVATE
		$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-fno-stack-protector>
	)
else()
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
endif()

# Emit Strict Aliasing
if (DEEP_USE_STRICT_ALIASING)
	target_compile_definitions(Deep PUBLIC DEEP_USE_STRICT_ALIASING)
endif()

# Emit Asserts
message(STATUS "${MESSAGE_PREFIX} USE_ASSERTS: ${DEEP_USE_ASSERTS}")
if (DEEP_USE_ASSERTS)
	target_compile_definitions(Deep PUBLIC DEEP_USE_ASSERTS)
endif()

# Emit C Bindings
message(STATUS "${MESSAGE_PREFIX} USE_C_BINDINGS: ${DEEP_USE_C_BINDINGS}")
if (DEEP_USE_C_BINDINGS)
	target_compile_definitions(Deep PUBLIC DEEP_USE_C_BINDINGS)
endif()

# Emit the instruction set definitions to ensure that child projects use the same settings even if they override the used instruction sets (a mismatch causes link errors)
function(EMIT_X86_INSTRUCTION_SET_DEFINITIONS)
	if (DEEP_USE_AVX512)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX512)
	endif()
	if (DEEP_USE_AVX2)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX2)
	endif()
	if (DEEP_USE_AVX)
		target_compile_definitions(Deep PUBLIC DEEP_USE_AVX)
	endif()
	if (DEEP_USE_SSE4_1)
		target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_1)
	endif()
	if (DEEP_USE_SSE4_2)
		target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_2)
	endif()
	if (DEEP_USE_LZCNT)
		target_compile_definitions(Deep PUBLIC DEEP_USE_LZCNT)
	endif()
	if (DEEP_USE_TZCNT)
		target_compile_definitions(Deep PUBLIC DEEP_USE_TZCNT)
	endif()
	if (DEEP_USE_F16C)
		target_compile_definitions(Deep PUBLIC DEEP_USE_F16C)
	endif()
	if (DEEP_USE_FMADD)
		target_compile_definitions(Deep PUBLIC DEEP_USE_FMADD)
	endif()
endfunction()

# Add the compiler commandline flags to select the right instruction sets
if (NOT DEEP_NO_SIMD_INTRINSICS)
	message(STATUS "${MESSAGE_PREFIX} Using SIMD Intrinsics")

	if (MSVC)
		if (DEEP_USE_AVX512)
			target_compile_options(Deep PUBLIC /arch:AVX512)
		elseif (DEEP_USE_AVX2)
			target_compile_options(Deep PUBLIC /arch:AVX2)
		elseif (DEEP_USE_AVX)
			target_compile_options(Deep PUBLIC /arch:AVX)
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	elseif (CLANG)
		if (DEEP_USE_WASM_SIMD)
			# Deep currently doesn't implement the WASM specific SIMD intrinsics so uses the SSE 4.2 intrinsics
			# See: https://emscripten.org/docs/porting/simd.html#webassembly-simd-intrinsics
			# Note that this does not require the browser to actually support SSE 4.2 it merely means that it can translate those instructions to WASM SIMD instructions
			target_compile_options(Jolt PUBLIC -msimd128 -msse4.2)
		else()
			if (DEEP_USE_AVX512)
				target_compile_options(Deep PUBLIC -mavx512f -mavx512cd -mavx512bw -mavx512dq -mavx512vl)
			elseif (DEEP_USE_AVX2)
				target_compile_options(Deep PUBLIC -mavx2)
			elseif (DEEP_USE_AVX)
				target_compile_options(Deep PUBLIC -mavx)
			endif()
		endif()
		EMIT_X86_INSTRUCTION_SET_DEFINITIONS()
	else()
		# TODO(randomuserhi)
		message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
	endif()
else()
	target_compile_definitions(Deep PUBLIC DEEP_NO_SIMD_INTRINSICS)
endif()