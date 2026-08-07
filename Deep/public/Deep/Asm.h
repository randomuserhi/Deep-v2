#pragma once

#include "Deep.h"
#include "Deep/Asm/Intrinsics.h"

DEEP_NAMESPACE_BEGIN

// TODO(randomuserhi): Implement cross-platform method
//
// Pauses CPU for spin locks via multiple calls to `_mm_pause`
inline void Spin() {
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
