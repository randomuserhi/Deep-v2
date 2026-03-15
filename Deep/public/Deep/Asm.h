#pragma once

#include "Deep.h"

#include <immintrin.h>

DEEP_NAMESPACE_BEGIN

// Pauses CPU for spin locks via multiple calls to `_mm_pause`
Deep_Inline void Spin() {
	// TODO(randomuserhi): Verify use of multiple pauses - benchmark and measure as always :P
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
	_mm_pause();
}

DEEP_NAMESPACE_END
