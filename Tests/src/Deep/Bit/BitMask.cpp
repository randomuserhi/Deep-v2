#include "Tests.h"

#define DEEP_TESTABLE

#include "Deep.h"
#include "Deep/Types.h"
#include "Deep/Bit/BitMask.h"

TEST(BitMask, Ops) {
	Deep::BitMask<32> mask{ 0 };
	EXPECT_EQ(mask.m_bits, 0);

	mask.Inverse();
	EXPECT_EQ(mask.m_bits, static_cast<uint32>(~0));

	mask &= 0b11u;
	EXPECT_EQ(mask.m_bits, static_cast<uint32>(0b11u));
}
