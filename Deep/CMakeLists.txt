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

# Detect wasm targets
if(CMAKE_SYSTEM_NAME STREQUAL "WASI")
    set(DEEP_TARGET_WASM ON)
else()
    set(DEEP_TARGET_WASM OFF)
endif()

# The module named used for imports when targeting wasm
set(DEEP_WASM_MODULE_NAME "deep_wasm" CACHE STRING "Name for the import wasm module")

set(DEEP_BUILD_SHARED_LIBS_DEFAULT OFF)
if(DEFINED BUILD_SHARED_LIBS AND NOT DEEP_TARGET_WASM)
    set(DEEP_BUILD_SHARED_LIBS_DEFAULT ${BUILD_SHARED_LIBS})
endif()
# When turning this option on, the library will be compiled as a shared library.
# Uses the value of BUILD_SHARED_LIBS if provided and is automatically set to OFF when targetting wasm.
option(DEEP_BUILD_SHARED_LIBS "Compile Deep as a native shared library" ${DEEP_BUILD_SHARED_LIBS_DEFAULT})

# When turning this option on, the library will be compiled using assertions. By default asserts are enabled in Debug build.
option(DEEP_USE_ASSERTS "Enable asserts" ${DEEP_USE_ASSERTS_DEFAULT})

# When turning this on, the library will be compiled with C++ exceptions enabled.
option(DEEP_CPP_EXCEPTIONS_ENABLED "Enable C++ exceptions" OFF)

# When turning this on, the library will not use any SIMD intrinsics
option(DEEP_NO_SIMD_INTRINSICS "Disable SIMD intrinsics" OFF)

# When turned on, the library will use floating point exceptions
option(DEEP_USE_FLOATING_POINT_EXCEPTIONS "Enable floating point exceptions" OFF)

# When turned on, the library will be compiled with RTTI
option(DEEP_USE_RTTI "Enable RTTI" OFF)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
option(DEEP_USE_SSE4_1 "Enable SSE4.1" ON)
option(DEEP_USE_SSE4_2 "Enable SSE4.2" ON)
option(DEEP_USE_AVX "Enable AVX" ON)
option(DEEP_USE_AVX2 "Enable AVX2" ON)
option(DEEP_USE_AVX512 "Enable AVX512" OFF)
option(DEEP_USE_LZCNT "Enable LZCNT" ON)
option(DEEP_USE_TZCNT "Enable TZCNT" ON)
option(DEEP_USE_F16C "Enable F16C" ON)
option(DEEP_USE_FMA "Enable FMA" ON)

set(DEEP_USE_C_BINDINGS_DEFAULT OFF)
if(DEEP_TARGET_WASM)
    set(DEEP_USE_C_BINDINGS_DEFAULT ON)
endif()
# When turning this option on, C bindings are exported.
# If wasm is being targeted, C bindings are enabled by default.
option(DEEP_USE_C_BINDINGS "Enable C Bindings" ${DEEP_USE_C_BINDINGS_DEFAULT})

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
		if (NOT (DEEP_BUILD_SHARED_LIBS))
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

# Validate options
if (DEEP_TARGET_WASM)
	if (DEEP_BUILD_SHARED_LIBS)
		message(FATAL_ERROR "${MESSAGE_PREFIX} Deep does not support shared libraries for wasm targets")
	endif()

	if (DEEP_CPP_EXCEPTIONS_ENABLED)
		message(FATAL_ERROR "${MESSAGE_PREFIX} Deep does not support exceptions for wasm targets")
	endif()
endif()

# Root location
set(DEEP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_ROOT "${DEEP_ROOT}/../ThirdParty")

# Obtain source files
file(GLOB_RECURSE DEEP_SRC_FILES CONFIGURE_DEPENDS "${DEEP_ROOT}/src/*.cpp")

# Create Deep lib
if (DEEP_BUILD_SHARED_LIBS)
	add_library(Deep SHARED ${DEEP_SRC_FILES})
else()
	add_library(Deep STATIC ${DEEP_SRC_FILES})
endif()
target_compile_features(Deep PUBLIC cxx_std_20)

# Add include directory
set(DEEP_INCLUDE_DIR "${DEEP_ROOT}/include")
target_include_directories(Deep PUBLIC "${DEEP_INCLUDE_DIR}/public_cpp")

if (DEEP_TARGET_WASM)
	# Add WASM intrinsics
	set(LLVM_WASM_ROOT "${THIRD_PARTY_ROOT}/llvm_wasm")
	target_include_directories(Deep PRIVATE "${LLVM_WASM_ROOT}/public")
endif()

# Handle shared library compilation
if (DEEP_BUILD_SHARED_LIBS)
	# Set default visibility to hidden
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)

	# Public define to instruct user code to import Core symbols (rather than use static linking)
	target_compile_definitions(Deep PUBLIC DEEP_SHARED_LIBRARY)

	# Private define to instruct the library to export symbols for shared linking
	target_compile_definitions(Deep PRIVATE DEEP_BUILD_SHARED_LIBRARY)
endif()

# Detect arch
if (CMAKE_SYSTEM_PROCESSOR)
    set(DEEP_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
elseif (CMAKE_CXX_COMPILER_TARGET)
    set(DEEP_ARCH "${CMAKE_CXX_COMPILER_TARGET}")
elseif (CMAKE_C_COMPILER_TARGET)
    set(DEEP_ARCH "${CMAKE_C_COMPILER_TARGET}")
else()
    message(FATAL_ERROR "Could not determine target architecture.")
endif()
string(TOLOWER "${DEEP_ARCH}" DEEP_ARCH)
if (DEEP_ARCH MATCHES "^(x86_64|amd64)$")
    set(DEEP_ARCH_X86_64 TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_X86_64)
elseif (DEEP_ARCH MATCHES "^(x86|i[3-6]86)$")
    set(DEEP_ARCH_X86 TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_X86)
elseif (DEEP_ARCH MATCHES "^(aarch64|arm64)$")
    set(DEEP_ARCH_ARM64 TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_ARM64)
elseif (DEEP_ARCH MATCHES "^arm")
    set(DEEP_ARCH_ARM TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_ARM)
elseif (DEEP_ARCH MATCHES "^wasm32$")
    set(DEEP_ARCH_WASM32 TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_WASM32)
elseif (DEEP_ARCH MATCHES "^wasm64$")
    set(DEEP_ARCH_WASM64 TRUE)
    target_compile_definitions(Deep PUBLIC DEEP_ARCH_WASM64)
else()
    message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported architecture: '${DEEP_ARCH}'")
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
	if (NOT DEEP_USE_FLOATING_POINT_EXCEPTIONS)
		target_compile_options(Deep PRIVATE -fno-trapping-math)
	endif()

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
				target_compile_options(Deep PRIVATE -gcodeview)
				target_link_options(Deep PRIVATE -fuse-ld=lld -Wl,/debug)
			else()
				check_linker_flag(CXX "-fuse-ld=link" HAS_LINK)
				if (HAS_LINK)
					target_compile_options(Deep PRIVATE -gcodeview)
					target_link_options(Deep PRIVATE -fuse-ld=link -Wl,/debug)
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
		target_compile_definitions(Deep PUBLIC DEEP_CPP_EXCEPTIONS_ENABLED=0)
		target_compile_options(Deep PRIVATE -fno-exceptions)
	else()
		target_compile_definitions(Deep PUBLIC DEEP_CPP_EXCEPTIONS_ENABLED=1)
		target_compile_options(Deep PRIVATE -fexceptions)
	endif()

	# Wasm Target
	message(STATUS "${MESSAGE_PREFIX} DEEP_TARGET_WASM: ${DEEP_TARGET_WASM}")
	if (DEEP_TARGET_WASM)
		message(STATUS "${MESSAGE_PREFIX} DEEP_WASM_MODULE_NAME: ${DEEP_WASM_MODULE_NAME}")
		target_compile_definitions(Deep PUBLIC DEEP_WASM_MODULE_NAME=\"${DEEP_WASM_MODULE_NAME}\")
	endif()

	# Remove buffer protections in release mode for maximum performance
	target_compile_options(Deep PRIVATE
		$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:-fno-stack-protector>
	)
else()
	message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
endif()

# Emit floating point exceptions
if (DEEP_USE_FLOATING_POINT_EXCEPTIONS)
	target_compile_definitions(Deep PUBLIC DEEP_USE_FLOATING_POINT_EXCEPTIONS)
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
function(EMIT_INSTRUCTION_SET_DEFINITIONS)
	if (NOT DEEP_NO_SIMD_INTRINSICS)
		if (DEEP_TARGET_WASM)
			target_compile_definitions(Deep PUBLIC DEEP_USE_WASM_SIMD128)
		else()
			if (DEEP_USE_AVX512)
				set(DEEP_USE_AVX512 ON)
				target_compile_definitions(Deep PUBLIC DEEP_USE_AVX512)
			endif()
			if (DEEP_USE_AVX2 OR DEEP_USE_AVX512)
				set(DEEP_USE_AVX2 ON)
				target_compile_definitions(Deep PUBLIC DEEP_USE_AVX2)
			endif()
			if (DEEP_USE_AVX OR DEEP_USE_AVX2)
				set(DEEP_USE_AVX ON)
				target_compile_definitions(Deep PUBLIC DEEP_USE_AVX)
			endif()
			if (DEEP_USE_SSE4_2 OR DEEP_USE_AVX)
				set(DEEP_USE_SSE4_2 ON)
				target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_2)
			endif()
			if (DEEP_USE_SSE4_1 OR DEEP_USE_SSE4_2)
				set(DEEP_USE_SSE4_1 ON)
				target_compile_definitions(Deep PUBLIC DEEP_USE_SSE4_1)
			endif()
			target_compile_definitions(Deep PUBLIC DEEP_USE_SSE2)

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
		endif()
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
		# TODO(randomuserhi): SSE architecture (refer to CLANG below)
	elseif (CLANG)
		if (DEEP_TARGET_WASM)
			# TODO(randomuserhi): Support emscripten, currently we support wasm on its own 
			#                     https://emscripten.org/docs/porting/simd.html#webassembly-simd-intrinsics
			target_compile_options(Deep PUBLIC -msimd128)
		elseif (DEEP_ARCH_X86 OR DEEP_ARCH_X86_64)
			if (DEEP_USE_AVX512)
				target_compile_options(Deep PUBLIC -mavx512f -mavx512cd -mavx512bw -mavx512dq -mavx512vl)
			elseif (DEEP_USE_AVX2)
				target_compile_options(Deep PUBLIC -mavx2)
			elseif (DEEP_USE_AVX)
				target_compile_options(Deep PUBLIC -mavx)
			elseif (DEEP_USE_SSE4_2)
				target_compile_options(Deep PUBLIC -msse4.2)
			elseif (DEEP_USE_SSE4_1)
				target_compile_options(Deep PUBLIC -msse4.1)		
			else()
				target_compile_options(Deep PUBLIC -msse2)
			endif()

			if (DEEP_USE_LZCNT)
				target_compile_options(Deep PUBLIC -mlzcnt)
			endif()

			if (DEEP_USE_TZCNT)
				target_compile_options(Deep PUBLIC -mbmi)
			endif()

			if (DEEP_USE_F16C)
				target_compile_options(Deep PUBLIC -mf16c)
			endif()

			if (DEEP_USE_FMA)
				target_compile_options(Deep PUBLIC -mfma)
			endif()
		else()
			message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported architecture: '${DEEP_ARCH}'")
		endif()
	else()
		# TODO(randomuserhi): GCC support
		message(FATAL_ERROR "${MESSAGE_PREFIX} Unsupported compiler")
	endif()
	
	EMIT_INSTRUCTION_SET_DEFINITIONS()
else()
	target_compile_definitions(Deep PUBLIC DEEP_NO_SIMD_INTRINSICS)
endif()