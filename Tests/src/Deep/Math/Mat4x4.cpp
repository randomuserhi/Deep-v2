#include "Tests.h"

#include "Deep/Math/Mat4x4.h"

TEST(Mat4, Equality) {
	const Deep::Mat4x4 a{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 b{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_EQ(a, b);

	const Deep::Mat4x4 c{
		1, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 d{
		0, 0, 0, 0, //
		0, 1, 0, 0, //
		0, 0, 1, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_NE(c, d);
}

TEST(Mat4, Multiplication) {
	const Deep::Mat4x4 a{
		1, 1, 1, 0, //
		0, 0, 0, 4, //
		0, 3, 2, 0, //
		0, 0, 0, 1  //
	};
	const Deep::Mat4x4 b{
		2, 3, 0, 1, //
		0, 0, 0, 0, //
		1, 0, 2, 0, //
		0, 0, 4, 0  //
	};
	const Deep::Mat4x4 c{
		3, 3, 2,  1, //
		0, 0, 16, 0, //
		2, 0, 4,  0, //
		0, 0, 4,  0  //
	};

	EXPECT_EQ((a * b), c);
}

TEST(Mat4, Determinant) {
	const Deep::Mat4x4 a{
		1, 1, 0, 0, //
		0, 0, 2, 0, //
		0, 3, 0, 0, //
		0, 0, 0, 1  //
	};

	EXPECT_EQ(a.determinant(), -6);
}
