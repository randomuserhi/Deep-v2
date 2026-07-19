#include "Tests.h"

#define DEEP_TESTABLE
#define DEEP_USE_TYPE_ALIASES
#include "Deep.h"
#include "Deep/Bit/IntegerBitMask.h"

TEST(IntegerBitMask, Ops) {
	Deep::IntegerBitMask<uint32> mask{ 0 };
	EXPECT_EQ(mask.m_bits, 0);

	mask.Inverse();
	EXPECT_EQ(mask.m_bits, static_cast<uint32>(~0));

	mask &= 0b11u;
	EXPECT_EQ(mask.m_bits, static_cast<uint32>(0b11u));
}
