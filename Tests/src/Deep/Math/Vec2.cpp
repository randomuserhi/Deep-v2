#include "Tests.h"

#include "Deep/Math/Vec2.h"

// TODO(randomuserhi): Test Vec2::Rotate and Vec2::Angle

TEST(Vec2, Equality) {
	const Deep::Vec2 a{ 1, 2 };
	const Deep::Vec2 b{ 1, 2 };

	EXPECT_EQ(a, b);
}

TEST(Vec2, Add) {
	Deep::Vec2 a{ 1, 2 };
	Deep::Vec2 b{ 2, 1 };
	Deep::Vec2 c = a + b;
	EXPECT_EQ(c.x, 3);
	EXPECT_EQ(c.y, 3);

	a += b;
	EXPECT_EQ(a.x, 3);
	EXPECT_EQ(a.y, 3);
}

TEST(Vec2, Sub) {
	Deep::Vec2 a{ 1, 2 };
	Deep::Vec2 b{ 2, 1 };
	Deep::Vec2 c = a - b;
	EXPECT_EQ(c.x, -1);
	EXPECT_EQ(c.y, 1);

	a -= b;
	EXPECT_EQ(a.x, -1);
	EXPECT_EQ(a.y, 1);
}

TEST(Vec2, Scale) {
	{
		Deep::Vec2 a{ 1, 2 };
		Deep::Vec2 b = a * 2;
		EXPECT_EQ(b.x, 2);
		EXPECT_EQ(b.y, 4);

		a *= 2;
		EXPECT_EQ(a.x, 2);
		EXPECT_EQ(a.y, 4);
	}

	{
		Deep::Vec2 a{ 1, 2 };
		Deep::Vec2 b = a / 2;
		EXPECT_EQ(b.x, 0.5f);
		EXPECT_EQ(b.y, 1.0f);

		a /= 2;
		EXPECT_EQ(a.x, 0.5f);
		EXPECT_EQ(a.y, 1.0f);
	}
}

TEST(Vec2, Dot) {
	Deep::Vec2 a{ 1, 2 };
	Deep::Vec2 b{ 2, 1 };
	float32 c = Deep::Vec2::Dot(a, b);
	float32 d = Deep::Vec2::Dot(b, a);
	EXPECT_EQ(c, 4);
	EXPECT_EQ(d, 4);
}

TEST(Vec2, Mul) {
	Deep::Vec2 a{ 1, 2 };
	Deep::Vec2 b{ 2, 1 };
	Deep::Vec2 c = a * b;
	EXPECT_EQ(c.x, 2.0f);
	EXPECT_EQ(c.y, 2.0f);

	a *= b;
	EXPECT_EQ(a.x, 2.0f);
	EXPECT_EQ(a.y, 2.0f);
}

TEST(Vec2, Div) {
	Deep::Vec2 a{ 1, 2 };
	Deep::Vec2 b{ 2, 1 };
	Deep::Vec2 c = a / b;
	EXPECT_EQ(c.x, 1.0f / 2.0f);
	EXPECT_EQ(c.y, 2.0f);

	a /= b;
	EXPECT_EQ(a.x, 1.0f / 2.0f);
	EXPECT_EQ(a.y, 2.0f);
}