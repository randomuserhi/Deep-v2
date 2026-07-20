#include "Tests.h"

#include "Deep.h"
#include "Deep/Types.h"
#include "Deep/Templates/SoALayout.h"

struct A {
	int32 m_value;
};

struct B {
	int64 m_value;
};

TEST(Templates, SoALayout) {
	constexpr size_t size = 10;
	Deep::SoALayout<A, B> layout{ size };

	EXPECT_EQ(layout.OffsetOf<A>(), 0);
	EXPECT_EQ(layout.OffsetOf<B>(), size * sizeof(A));

	EXPECT_EQ(layout.SizeOf<A>(), size * sizeof(A));
	EXPECT_EQ(layout.SizeOf<B>(), size * sizeof(B));

	EXPECT_EQ(layout.m_Size(), size * sizeof(A) + size * sizeof(B));
}
