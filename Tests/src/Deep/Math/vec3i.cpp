#include "Tests.h"

#define DEEP_USE_TYPE_ALIASES
#include "Deep/Math/Vec3i.h"

TEST(Vec3i, XYZ) {
	Deep::Vec3i a{ 1, 2, 3 };
	const Deep::Vec3i b{ 1, 1, 7 };

	a.XYZ(b);

	EXPECT_EQ(a._w, 3);
	EXPECT_EQ(a, b);
}

TEST(Vec3i, Equality) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 1, 2, 3 };

	EXPECT_EQ(a, b);
}

TEST(Vec3i, Add) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 4, 3, 2 };
	Deep::Vec3i c = a + b;
	EXPECT_EQ(c.x, 5);
	EXPECT_EQ(c.y, 5);
	EXPECT_EQ(c.z, 5);

	a += b;
	EXPECT_EQ(a.x, 5);
	EXPECT_EQ(a.y, 5);
	EXPECT_EQ(a.z, 5);
}

TEST(Vec3i, Sub) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 4, 3, 2 };
	Deep::Vec3i c = a - b;
	EXPECT_EQ(c.x, -3);
	EXPECT_EQ(c.y, -1);
	EXPECT_EQ(c.z, 1);

	a -= b;
	EXPECT_EQ(a.x, -3);
	EXPECT_EQ(a.y, -1);
	EXPECT_EQ(a.z, 1);
}

TEST(Vec3i, Scale) {
	{
		Deep::Vec3i a{ 1, 2, 3 };
		Deep::Vec3i b = a * 2;
		EXPECT_EQ(b.x, 2);
		EXPECT_EQ(b.y, 4);
		EXPECT_EQ(b.z, 6);

		a *= 2;
		EXPECT_EQ(a.x, 2);
		EXPECT_EQ(a.y, 4);
		EXPECT_EQ(a.z, 6);
	}

	{
		Deep::Vec3i a{ 1, 2, 3 };
		Deep::Vec3i b = a / 2;
		EXPECT_EQ(b.x, 0);
		EXPECT_EQ(b.y, 1);
		EXPECT_EQ(b.z, 1);

		a /= 2;
		EXPECT_EQ(a.x, 0);
		EXPECT_EQ(a.y, 1);
		EXPECT_EQ(a.z, 1);
	}
}

TEST(Vec3i, Dot) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 4, 3, 2 };
	int32 c = Deep::Vec3i::s_Dot(a, b);
	int32 d = Deep::Vec3i::s_Dot(b, a);
	EXPECT_EQ(c, 16);
	EXPECT_EQ(d, 16);
}

TEST(Vec3i, Mul) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 4, 3, 2 };
	Deep::Vec3i c = a * b;
	EXPECT_EQ(c.x, 4);
	EXPECT_EQ(c.y, 6);
	EXPECT_EQ(c.z, 6);

	a *= b;
	EXPECT_EQ(a.x, 4);
	EXPECT_EQ(a.y, 6);
	EXPECT_EQ(a.z, 6);
}

TEST(Vec3i, Div) {
	Deep::Vec3i a{ 1, 2, 3 };
	Deep::Vec3i b{ 4, 3, 2 };
	Deep::Vec3i c = a / b;
	EXPECT_EQ(c.x, 0);
	EXPECT_EQ(c.y, 0);
	EXPECT_EQ(c.z, 1);

	a /= b;
	EXPECT_EQ(a.x, 0);
	EXPECT_EQ(a.y, 0);
	EXPECT_EQ(a.z, 1);
}

TEST(Vec3i, ManhattanDistance) {
	Deep::Vec3i a{ 1, 2, 3 };
	EXPECT_EQ(a.m_ManhattanDistance(), 6);
}
