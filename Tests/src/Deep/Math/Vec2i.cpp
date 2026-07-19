#include "Tests.h"

#include "Deep/Math/Vec2i.h"

TEST(Vec2i, Equality) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 1, 2 };

	EXPECT_EQ(a, b);
}

TEST(Vec2i, Add) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 2, 1 };
	Deep::Vec2i c = a + b;
	EXPECT_EQ(c.x, 3);
	EXPECT_EQ(c.y, 3);

	a += b;
	EXPECT_EQ(a.x, 3);
	EXPECT_EQ(a.y, 3);
}

TEST(Vec2i, Sub) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 2, 1 };
	Deep::Vec2i c = a - b;
	EXPECT_EQ(c.x, -1);
	EXPECT_EQ(c.y, 1);

	a -= b;
	EXPECT_EQ(a.x, -1);
	EXPECT_EQ(a.y, 1);
}

TEST(Vec2i, Scale) {
	{
		Deep::Vec2i a{ 1, 2 };
		Deep::Vec2i b = a * 2;
		EXPECT_EQ(b.x, 2);
		EXPECT_EQ(b.y, 4);

		a *= 2;
		EXPECT_EQ(a.x, 2);
		EXPECT_EQ(a.y, 4);
	}

	{
		Deep::Vec2i a{ 1, 2 };
		Deep::Vec2i b = a / 2;
		EXPECT_EQ(b.x, 0);
		EXPECT_EQ(b.y, 1);

		a /= 2;
		EXPECT_EQ(a.x, 0);
		EXPECT_EQ(a.y, 1);
	}
}

TEST(Vec2i, Dot) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 2, 1 };
	int32 c = Deep::Vec2i::s_Dot(a, b);
	int32 d = Deep::Vec2i::s_Dot(b, a);
	EXPECT_EQ(c, 4);
	EXPECT_EQ(d, 4);
}

TEST(Vec2i, Mul) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 2, 1 };
	Deep::Vec2i c = a * b;
	EXPECT_EQ(c.x, 2);
	EXPECT_EQ(c.y, 2);

	a *= b;
	EXPECT_EQ(a.x, 2);
	EXPECT_EQ(a.y, 2);
}

TEST(Vec2i, Div) {
	Deep::Vec2i a{ 1, 2 };
	Deep::Vec2i b{ 2, 1 };
	Deep::Vec2i c = a / b;
	EXPECT_EQ(c.x, 0);
	EXPECT_EQ(c.y, 2);

	a /= b;
	EXPECT_EQ(a.x, 0);
	EXPECT_EQ(a.y, 2);
}

TEST(Vec2i, ManhattanDistance) {
	Deep::Vec2i a{ 1, 2 };
	EXPECT_EQ(a.m_ManhattanDistance(), 3);
}
