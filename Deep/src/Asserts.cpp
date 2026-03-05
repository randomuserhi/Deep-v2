/**
 * Asserts.cpp
 */

#include <Deep.h>

#ifdef DEEP_USE_ASSERTS

#include <iostream>

DEEP_NAMESPACE_BEGIN

// A dummy assert fail implementation that simply just logs to console. You should provide your own.
bool OnAssertFailImpl(const char* in_expression, const char* in_file, int32 in_line, const char* in_message) {
    std::cout << in_file << ":" << in_line << " Assertion Failed (" << in_expression << ") "
              << (in_message != nullptr ? in_message : "") << std::endl;

    return true;
}

bool (*g_onAssertFail)(const char* in_expression, const char* in_file, int32 in_line,
                       const char* in_message) = &OnAssertFailImpl;

DEEP_NAMESPACE_END

#endif
