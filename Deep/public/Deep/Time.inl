#pragma once

#include "Deep/Time.h"

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <chrono>
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

int64 Epoch() {
	return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
	    .time_since_epoch()
	    .count();
}

DEEP_NAMESPACE_END
