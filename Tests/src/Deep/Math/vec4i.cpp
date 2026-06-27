#include "Tests.h"

#include "Deep/Math/Vec4i.h"

TEST(Vec4i, Equality) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 1, 2, 3, 4 };

	EXPECT_EQ(a, b);
}

TEST(Vec4i, Add) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 4, 3, 2, 1 };
	Deep::Vec4i c = a + b;
	EXPECT_EQ(c.x, 5);
	EXPECT_EQ(c.y, 5);
	EXPECT_EQ(c.z, 5);
	EXPECT_EQ(c.w, 5);

	a += b;
	EXPECT_EQ(a.x, 5);
	EXPECT_EQ(a.y, 5);
	EXPECT_EQ(a.z, 5);
	EXPECT_EQ(a.w, 5);
}

TEST(Vec4i, Sub) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 4, 3, 2, 1 };
	Deep::Vec4i c = a - b;
	EXPECT_EQ(c.x, -3);
	EXPECT_EQ(c.y, -1);
	EXPECT_EQ(c.z, 1);
	EXPECT_EQ(c.w, 3);

	a -= b;
	EXPECT_EQ(a.x, -3);
	EXPECT_EQ(a.y, -1);
	EXPECT_EQ(a.z, 1);
	EXPECT_EQ(a.w, 3);
}

TEST(Vec4i, Scale) {
	{
		Deep::Vec4i a{ 1, 2, 3, 4 };
		Deep::Vec4i b = a * 2;
		EXPECT_EQ(b.x, 2);
		EXPECT_EQ(b.y, 4);
		EXPECT_EQ(b.z, 6);
		EXPECT_EQ(b.w, 8);

		a *= 2;
		EXPECT_EQ(a.x, 2);
		EXPECT_EQ(a.y, 4);
		EXPECT_EQ(a.z, 6);
		EXPECT_EQ(a.w, 8);
	}

	{
		Deep::Vec4i a{ 1, 2, 3, 4 };
		Deep::Vec4i b = a / 2;
		EXPECT_EQ(b.x, 0);
		EXPECT_EQ(b.y, 1);
		EXPECT_EQ(b.z, 1);
		EXPECT_EQ(b.w, 2);

		a /= 2;
		EXPECT_EQ(a.x, 0);
		EXPECT_EQ(a.y, 1);
		EXPECT_EQ(a.z, 1);
		EXPECT_EQ(a.w, 2);
	}
}

TEST(Vec4i, Dot) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 4, 3, 2, 1 };
	int32 c = Deep::Vec4i::s_Dot(a, b);
	int32 d = Deep::Vec4i::s_Dot(b, a);
	EXPECT_EQ(c, 20);
	EXPECT_EQ(d, 20);
}

TEST(Vec4i, Mul) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 4, 3, 2, 1 };
	Deep::Vec4i c = a * b;
	EXPECT_EQ(c.x, 4);
	EXPECT_EQ(c.y, 6);
	EXPECT_EQ(c.z, 6);
	EXPECT_EQ(c.w, 4);

	a *= b;
	EXPECT_EQ(a.x, 4);
	EXPECT_EQ(a.y, 6);
	EXPECT_EQ(a.z, 6);
	EXPECT_EQ(a.w, 4);
}

TEST(Vec4i, Div) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	Deep::Vec4i b{ 4, 3, 2, 1 };
	Deep::Vec4i c = a / b;
	EXPECT_EQ(c.x, 0);
	EXPECT_EQ(c.y, 0);
	EXPECT_EQ(c.z, 1);
	EXPECT_EQ(c.w, 4);

	a /= b;
	EXPECT_EQ(a.x, 0);
	EXPECT_EQ(a.y, 0);
	EXPECT_EQ(a.z, 1);
	EXPECT_EQ(a.w, 4);
}

TEST(Vec4i, ManhattanDistance) {
	Deep::Vec4i a{ 1, 2, 3, 4 };
	EXPECT_EQ(a.m_ManhattanDistance(), 10);
}
