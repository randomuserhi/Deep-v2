#include "Deep.h"

#if defined(DEEP_USE_SSE2)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#include <immintrin.h> // IWYU pragma: export
DEEP_SUPPRESS_WARNINGS_STD_END
#elif defined(DEEP_USE_NEON)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#ifdef DEEP_COMPILER_MSVC
		#include <intrin.h>     // IWYU pragma: export
		#include <arm64_neon.h> // IWYU pragma: export
	#else
		#include <arm_neon.h> // IWYU pragma: export
	#endif
DEEP_SUPPRESS_WARNINGS_STD_END
#elif defined(DEEP_USE_WASM_SIMD128)
DEEP_SUPPRESS_WARNINGS_WASM_SIMD128_BEGIN
	#include "wasm_simd128.h" // IWYU pragma: export
DEEP_SUPPRESS_WARNINGS_WASM_SIMD128_END
#endif
