/**
 * Asserts.cpp
 */

#include "Deep.h" // IWYU pragma: keep

#if defined(DEEP_PLATFORM_WASM)

extern "C" {
DEEP_WASM_IMPORT("on_assert_fail_impl")
Deep::int32 DeepWasm_OnAssertFailImpl(const char* in_expression, const char* in_file, Deep::int32 in_line,
                                      const char* in_message);
}

#endif

#ifdef DEEP_USE_ASSERTS

	#include <iostream>

DEEP_NAMESPACE_BEGIN

// A dummy assert fail implementation that simply just logs to console. You should provide your own.
bool OnAssertFailImpl(const char* in_expression, const char* in_file, int32 in_line, const char* in_message) {
	#if defined(DEEP_PLATFORM_WASM)
	return DeepWasm_OnAssertFailImpl(in_expression, in_file, in_line, in_message) != 0;
	#else
	std::cout << in_file << ":" << in_line << " Assertion Failed (" << in_expression << ") "
			  << (in_message != nullptr ? in_message : "") << std::endl;
	return true;
	#endif
}

AssertFailHandler g_onAssertFail = &OnAssertFailImpl;

DEEP_NAMESPACE_END

#endif
